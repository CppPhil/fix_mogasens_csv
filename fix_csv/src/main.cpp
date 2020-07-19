#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include <pl/algo/clamp.hpp>
#include <pl/numeric.hpp>
#include <pl/string_view.hpp>

#include <csv.hpp>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "adjust_hardware_timestamp.hpp"
#include "columns.hpp"
#include "delete_non_bosch_sensors.hpp"
#include "remove_zeros_from_field.hpp"

int main(int argc, char* argv[])
{
  constexpr int expectedArgumentCount{2};
  constexpr int csvPathIndex{1};

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

  try {
    csv::CSVReader csvReader{
      csv::string_view{csvPath.c_str(), csvPath.size()},
      csv::CSVFormat{}
        .delimiter(',')
        .quote('"')
        .header_row(0)
        .detect_bom(true)
        .variable_columns(csv::VariableColumnPolicy::KEEP)};
    std::vector<std::vector<std::string>> data(
      csvReader.begin(), csvReader.end());
    const std::string outputFilePath{[csvPath] {
      std::string buffer(
        csvPath.c_str(), csvPath.size() - csvFileExtension.size());
      buffer += "_out";
      buffer.append(csvFileExtension.begin(), csvFileExtension.end());
      return buffer;
    }()};
    std::ofstream     outputFileStream{
      outputFilePath, std::ios_base::out | std::ios_base::trunc};

    if (!outputFileStream) {
      fmt::print(stderr, "Couldn't open \"{}\" for writing!\n", outputFilePath);
      return EXIT_FAILURE;
    }

    fmc::deleteNonBoschSensors(&data);

    auto csvWriter = csv::make_csv_writer(outputFileStream);
    csvWriter << csvReader.get_col_names();

    {
      std::size_t   rowCount{2};
      std::uint64_t overflowCount{0};

      for (std::vector<std::string>& currentRow : data) {
        std::size_t columnCount{1};

        constexpr std::size_t startRowExpectedSize{11};
        const std::size_t     expectedRowSize{
          (rowCount == 2) ? (startRowExpectedSize) : (10)};

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
              fmt::print(stderr, "set last hw ts to: {}\n", beforeChange);
              lastHardwareTimestampWritten = currentField;
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
                    asFloat,
                    gyroscopeRangeLowerBound,
                    gyroscopeRangeUpperBound);
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

    fmt::print("Successfully wrote data to \"{}\".\n", outputFilePath);
  }
  catch (const std::runtime_error& exception) {
    const pl::string_view errorMessage{exception.what()};

    if (errorMessage.starts_with("Cannot open file")) {
      fmt::print(
        stderr, "Couldn't open CSV file given: \"{}\"!\n", errorMessage);
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
