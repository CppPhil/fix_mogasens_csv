#include <cstdio>
#include <cstdlib>

#include <fstream>
#include <ios>
#include <iostream>

#include <pl/algo/clamp.hpp>
#include <pl/numeric.hpp>
#include <pl/string_view.hpp>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <csv.hpp>

#include "cl/dos2unix.hpp"
#include "cl/fs/file.hpp"
#include "cl/fs/file_stream.hpp"
#include "cl/read_csv_file.hpp"
#include "cl/to_string.hpp"

#include "adjust_hardware_timestamp.hpp"
#include "columns.hpp"
#include "delete_non_bosch_sensors.hpp"
#include "remove_zeros_from_field.hpp"

namespace fmc {
namespace {
[[nodiscard]] bool createBackupFile(
  const std::string& csvFilePath,
  const std::string& backupFilePath)
{
  const cl::fs::File csvFile{cl::fs::Path{csvFilePath}};
  return csvFile.copyTo(cl::fs::Path{backupFilePath});
}

[[nodiscard]] bool restoreFromBackup(
  const std::string& csvFilePath,
  const std::string& backupFilePath)
{
  const cl::fs::Path modifiedFilePath{csvFilePath};
  cl::fs::File       csvFile{modifiedFilePath};
  if (!csvFile.remove()) {
    fmt::print(stderr, "Couldn't delete file \"{}\".\n", modifiedFilePath);
    return false;
  }
  cl::fs::File backupFile{cl::fs::Path{backupFilePath}};
  if (!backupFile.moveTo(modifiedFilePath)) {
    fmt::print(
      stderr,
      "Couldn't move file \"{}\" to \"{}\"!\n",
      backupFilePath,
      modifiedFilePath);
    return false;
  }
  return true;
}

[[nodiscard]] bool convertToUnixLineEndings(const std::string& csvPath)
{
  cl::fs::File csvFile{cl::fs::Path{csvPath}};
  std::vector<pl::byte>
    crlfCsvData{}; // The CR LF line terminated data will go here.
  {
    cl::Expected<cl::fs::FileStream> fileStream{
      cl::fs::FileStream::create(csvFile, cl::fs::FileStream::Read)};
    if (!fileStream.has_value()) {
      fmt::print(
        stderr,
        "Couldn't open file stream for reading to file \"{}\"!\n",
        csvPath);
      return false;
    }
    crlfCsvData = fileStream->readAll();
  }
  // Convert to Unix line endings.
  const std::vector<pl::byte> lfCsvData{
    cl::dos2unix(crlfCsvData.data(), crlfCsvData.size())};
  if (!csvFile.remove()) {
    fmt::print(stderr, "Couldn't delete file \"{}\"!\n", csvPath);
    return false;
  }
  if (!csvFile.create()) {
    fmt::print(
      stderr,
      "Couldn't create file \"{}\" after having deleted it!\n",
      csvPath);
    return false;
  }
  cl::Expected<cl::fs::FileStream> filestream{
    cl::fs::FileStream::create(csvFile, cl::fs::FileStream::Write)};
  if (!filestream.has_value()) {
    fmt::print(
      stderr,
      "Couldn't open file stream for writing for file \"{}\"!\n",
      csvPath);
    return false;
  }
  // Write the Unix line endings back to the file.
  if (!filestream->write(lfCsvData.data(), lfCsvData.size())) {
    fmt::print(stderr, "Couldn't write LF data to file \"{}\"!\n", csvPath);
    return false;
  }
  return true;
}
} // namespace
} // namespace fmc

int main(int argc, char* argv[])
{
  constexpr int expectedArgumentCount{2};
  constexpr int csvPathIndex{1};

  std::setbuf(stdout, nullptr);
  std::setbuf(stderr, nullptr);
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  if (argc != expectedArgumentCount) {
    fmt::print(
      stderr,
      "Unexpected command line argument count: {}, expected: {}!\n",
      argc - 1,
      expectedArgumentCount - 1);
    return EXIT_FAILURE;
  }

  const pl::string_view csvPath{argv[csvPathIndex]};
  using namespace pl::string_view_literals;
  static constexpr auto csvFileExtension = ".csv"_sv;

  if (!csvPath.ends_with(csvFileExtension)) {
    fmt::print(
      stderr, "\"{}\" doesn't end with \"{}\"!\n", csvPath, csvFileExtension);
    return EXIT_FAILURE;
  }

  const std::string csvPathString(csvPath.to_string());
  const std::string csvBackupPath{csvPathString + "_backup"};
  if (!fmc::createBackupFile(csvPathString, csvBackupPath)) {
    fmt::print(
      stderr,
      "Couldn't copy \"{}\" to \"{}\".\n",
      csvPathString,
      csvBackupPath);
    return EXIT_FAILURE;
  }
  if (!fmc::convertToUnixLineEndings(csvPathString)) {
    fmt::print(
      stderr, "Couldn't convert \"{}\" to Unix line endings.\n", csvPathString);
    return EXIT_FAILURE;
  }

  std::vector<std::string>                            columnNames{};
  cl::Expected<std::vector<std::vector<std::string>>> expectedData{
    cl::readCsvFile(csvPath, &columnNames, cl::CsvFileKind::Raw)};

  if (!fmc::restoreFromBackup(csvPathString, csvBackupPath)) {
    return EXIT_FAILURE;
  }

  if (!expectedData.has_value()) {
    fmt::print(
      stderr, "Couldn't read \"{}\": \"{}\"\n", csvPath, expectedData.error());
    return EXIT_FAILURE;
  }

  std::vector<std::vector<std::string>> data{std::move(expectedData).value()};
  const std::string                     outputFilePath{[csvPath] {
    std::string buffer(
      csvPath.c_str(), csvPath.size() - csvFileExtension.size());
    buffer += "_out";
    buffer.append(csvFileExtension.begin(), csvFileExtension.end());
    return buffer;
  }()};
  std::ofstream                         outputFileStream{
    outputFilePath, std::ios_base::out | std::ios_base::trunc};

  if (!outputFileStream) {
    fmt::print(stderr, "Couldn't open \"{}\" for writing!\n", outputFilePath);
    return EXIT_FAILURE;
  }

  fmc::deleteNonBoschSensors(&data);

  auto csvWriter = csv::make_csv_writer(outputFileStream);
  csvWriter << columnNames;

  {
    std::size_t   rowCount{2};
    std::uint64_t overflowCount{0};

    for (std::vector<std::string>& currentRow : data) {
      std::size_t columnCount{1};

      constexpr std::size_t expectedRowSize{10};

      if (currentRow.size() != expectedRowSize) {
        fmt::print(
          stderr,
          "Row {} was of size {}, but size {} was expected!\n",
          rowCount,
          currentRow.size(),
          expectedRowSize);
      }

      for (std::string& currentField : currentRow) {
        if (columnCount == fmc::hardwareTimestampColumn) {
          constexpr std::uint64_t overflowThreshold{65532U};
          const std::string       beforeChange{currentField};
          static std::string      lastHardwareTimestampBeforeChange{};
          static std::string      lastHardwareTimestampWritten{};

          if (lastHardwareTimestampBeforeChange == beforeChange) {
            currentField = lastHardwareTimestampWritten;
          }
          else {
            fmc::adjustHardwareTimestamp(
              &currentField, overflowThreshold, &overflowCount);
            lastHardwareTimestampBeforeChange = beforeChange;
            lastHardwareTimestampWritten      = currentField;
          }
        }
        else if (pl::is_between(
                   columnCount,
                   fmc::accelerometerXColumn,
                   fmc::gyroscopeZColumn)) {
          fmc::removeZerosFromField(&currentField);

          const auto printError
            = [columnCount, rowCount](const char* errorMessage) {
                fmt::print(
                  stderr,
                  "Could not parse value at column {}, row {} as an integer: "
                  "\"{}\"\n",
                  columnCount,
                  rowCount,
                  errorMessage);
              };

          try {
            long long asInteger{std::stoll(currentField)};

            if (!pl::is_between(
                  asInteger,
                  static_cast<long long>(INT16_MIN),
                  static_cast<long long>(INT16_MAX))) {
              fmt::print(
                stderr,
                "Channel value {} at column {}, row {} was not within {} to "
                "{}, "
                "clamping value!\n",
                asInteger,
                columnCount,
                rowCount,
                INT16_MIN,
                INT16_MAX);

              asInteger = pl::algo::clamp(
                asInteger,
                static_cast<long long>(INT16_MIN),
                static_cast<long long>(INT16_MAX));
            }

            long double asFloat{static_cast<long double>(asInteger)};

            if (pl::is_between(
                  columnCount,
                  fmc::accelerometerXColumn,
                  fmc::accelerometerZColumn)) {
              constexpr long double accelerometerSensitivity{16384.0L};
              asFloat /= accelerometerSensitivity;
              constexpr long double accelerometerRangeLowerBound{-2.0L};
              constexpr long double accelerometerRangeUpperBound{2.0L};

              if (!pl::is_between(
                    asFloat,
                    accelerometerRangeLowerBound,
                    accelerometerRangeUpperBound)) {
                fmt::print(
                  stderr,
                  "Accelerometer value {} at column {}, row {} was not "
                  "within {} to {}, clamping value!\n",
                  asFloat,
                  columnCount,
                  rowCount,
                  accelerometerRangeLowerBound,
                  accelerometerRangeUpperBound);
                asFloat = pl::algo::clamp(
                  asFloat,
                  accelerometerRangeLowerBound,
                  accelerometerRangeUpperBound);
              }
            }
            else if (pl::is_between(
                       columnCount,
                       fmc::gyroscopeXColumn,
                       fmc::gyroscopeZColumn)) {
              constexpr long double gyroscopeDivisor{16.4L};
              asFloat /= gyroscopeDivisor;
              constexpr long double gyroscopeRangeLowerBound{-2000.0L};
              constexpr long double gyroscopeRangeUpperBound{2000.0L};

              if (!pl::is_between(
                    asFloat,
                    gyroscopeRangeLowerBound,
                    gyroscopeRangeUpperBound)) {
                fmt::print(
                  stderr,
                  "Gyroscope value {} at column {}, row {} was not "
                  "within {} to {}, clamping value!\n",
                  asFloat,
                  columnCount,
                  rowCount,
                  gyroscopeRangeLowerBound,
                  gyroscopeRangeUpperBound);
                asFloat = pl::algo::clamp(
                  asFloat, gyroscopeRangeLowerBound, gyroscopeRangeUpperBound);
              }
            }

            currentField = std::to_string(asFloat);
          }
          catch (const std::invalid_argument& exception) {
            printError(exception.what());
            return EXIT_FAILURE;
          }
          catch (const std::out_of_range& exception) {
            printError(exception.what());
            return EXIT_FAILURE;
          }
        }

        ++columnCount;
      }

      csvWriter << currentRow;
      ++rowCount;
    }
  }

  if (!fmc::convertToUnixLineEndings(outputFilePath)) { return EXIT_FAILURE; }

  fmt::print("Successfully wrote data to \"{}\".\n", outputFilePath);

  return EXIT_SUCCESS;
}
