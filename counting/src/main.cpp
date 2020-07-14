#include <cstdio>
#include <cstdlib>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "above_threshold.hpp"
#include "data_set.hpp"
#include "read_csv_file.hpp"

int main(int argc, char* argv[])
{
  constexpr int expectedArgumentCount{2};
  constexpr int filePathIndex{1};

  if (argc != expectedArgumentCount) {
    fmt::print(
      stderr,
      "{}: {} command line arguments given, but {} were expected, exiting.\n",
      argv[0],
      argc,
      expectedArgumentCount);
    return EXIT_FAILURE;
  }

  const pl::string_view filePath{argv[filePathIndex]};
  const cl::Expected<std::vector<std::vector<std::string>>> readFileResult{
    cl::readCsvFile(filePath)};

  if (!readFileResult.has_value()) {
    fmt::print(
      stderr,
      "{}: couldn't read \"{}\", error: \"{}\".\n",
      argv[0],
      filePath,
      readFileResult.error());
    return EXIT_FAILURE;
  }

  const cl::Expected<cl::DataSet> expectedDataSet{
    cl::DataSet::create(filePath.to_string(), readFileResult.value())};

  if (!expectedDataSet.has_value()) {
    fmt::print(
      stderr,
      "{}: couldn't create data set from \"{}\", error: \"{}\".\n",
      argv[0],
      filePath,
      expectedDataSet.error());
    return EXIT_FAILURE;
  }

  fmt::print("{}: processing file \"{}\"\n", argv[0], filePath);

  const cl::DataSet& dataSet{expectedDataSet.value()};

  constexpr long double            threshold{1.99L};
  const std::vector<cl::DataPoint> aboveThresholdDataPoints{
    ctg::aboveThreshold(dataSet, threshold)};

  fmt::print(
    "{} data points above {} / below {} were found.\n",
    aboveThresholdDataPoints.size(),
    threshold,
    -threshold);

  for (const cl::DataPoint& dataPoint : aboveThresholdDataPoints) {
    fmt::print("{}\n", dataPoint);
  }

  return EXIT_SUCCESS;
}
