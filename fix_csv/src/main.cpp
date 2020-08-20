#include <cstdio>
#include <cstdlib>

#include <fstream>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/algo/clamp.hpp>
#include <pl/numeric.hpp>
#include <pl/string_view.hpp>

#include "cl/read_csv_file.hpp"
#include "cl/use_unbuffered_io.hpp"

#include "adjust_hardware_timestamp.hpp"
#include "columns.hpp"
#include "convert_to_unix_line_endings.hpp"
#include "create_backup_file.hpp"
#include "delete_non_bosch_sensors.hpp"
#include "delete_out_of_bounds_values.hpp"
#include "remove_zeros_from_field.hpp"
#include "restore_from_backup.hpp"
#include "write_file.hpp"

int main(int argc, char* argv[])
{
  constexpr int         expectedArgumentCount{2};
  constexpr int         csvPathIndex{1};
  const pl::string_view thisApplication{argv[0]};

  cl::useUnbufferedIo();

  fmt::print("{}: starting.\n", thisApplication);

  if (argc != expectedArgumentCount) {
    fmt::print(
      stderr,
      "{}: Unexpected command line argument count: {}, expected: {}!\n",
      thisApplication,
      argc - 1,
      expectedArgumentCount - 1);
    return EXIT_FAILURE;
  }

  const pl::string_view csvPath{argv[csvPathIndex]};
  using namespace pl::string_view_literals;
  static constexpr auto csvFileExtension = ".csv"_sv;

  if (!csvPath.ends_with(csvFileExtension)) {
    fmt::print(
      stderr,
      "{}: \"{}\" doesn't end with \"{}\"!\n",
      thisApplication,
      csvPath,
      csvFileExtension);
    return EXIT_FAILURE;
  }

  const std::string csvPathString(csvPath.to_string());
  const std::string csvBackupPath{csvPathString + "_backup"};
  if (!fmc::createBackupFile(csvPathString, csvBackupPath)) {
    fmt::print(
      stderr,
      "{}: Couldn't copy \"{}\" to \"{}\".\n",
      thisApplication,
      csvPathString,
      csvBackupPath);
    return EXIT_FAILURE;
  }
  if (!fmc::convertToUnixLineEndings(csvPathString)) {
    fmt::print(
      stderr,
      "{}: Couldn't convert \"{}\" to Unix line endings.\n",
      thisApplication,
      csvPathString);
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
      stderr,
      "{}: Couldn't read \"{}\": \"{}\"\n",
      thisApplication,
      csvPath,
      expectedData.error());
    return EXIT_FAILURE;
  }

  std::vector<std::vector<std::string>> data{std::move(expectedData).value()};

  fmc::deleteNonBoschSensors(&data);

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
          const auto nextRowIndex{
            rowCount}; /* rowCount is the index of the next row */

          if (nextRowIndex < data.size()) { /* if not the last row */
            const std::string nextRowValue{
              data.at(nextRowIndex)
                .at(cl::column_index<cl::Column::HardwareTimestamp>)};

            fmt::print(
              stderr,
              "currentField: \"{}\"\n"
              "nextRowIndex: {}\n"
              "nextRowValue: \"{}\"\n"
              "overflowCount: {}\n",
              currentField,
              nextRowIndex,
              nextRowValue,
              overflowCount);

            fmc::adjustHardwareTimestamp(
              /* cellContent */ &currentField,
              /* nextRowHardwareTimestamp */ nextRowValue,
              /* overflowCount */ &overflowCount);

            fmt::print(
              stderr,
              "AFTERWARD\n"
              "currentField: \"{}\"\n"
              "nextRowIndex: {}\n"
              "nextRowValue: \"{}\"\n"
              "overflowCount: {}\n\n",
              currentField,
              nextRowIndex,
              nextRowValue,
              overflowCount);
          }
          else {
            fmt::print(
              stderr, "DIDN'T GO IN THERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
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
      ++rowCount;
    }
  }

  if (const cl::Expected<void> expected{fmc::deleteOutOfBoundsValues(&data)};
      !expected.has_value()) {
    fmt::print(
      stderr,
      "{}: Couldn't erase out-of-bounds values from \"{}\", error message: "
      "\"{}\"!\n",
      thisApplication,
      csvPath,
      expected.error());
    return EXIT_FAILURE;
  }

  if (!fmc::writeFile(csvPath, csvFileExtension, columnNames, data)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
