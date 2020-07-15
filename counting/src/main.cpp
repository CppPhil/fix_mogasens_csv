#include <cstdio>
#include <cstdlib>

#include <fstream>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "cl/data_set.hpp"
#include "cl/read_csv_file.hpp"

#include "above_threshold.hpp"
#include "percentage_of.hpp"

int main(int argc, char* argv[])
{
  constexpr int minimumArgumentCount{2};

  if (argc < minimumArgumentCount) {
    fmt::print(
      stderr,
      "{}: {} command line arguments given, but at least {} were expected, "
      "exiting.\n",
      argv[0],
      argc,
      minimumArgumentCount);
    return EXIT_FAILURE;
  }

  const pl::string_view logFilePath{"above_threshold_log.txt"};
  std::ofstream         ofs{
    logFilePath.c_str(), std::ios_base::out | std::ios_base::trunc};

  if (!ofs) {
    fmt::print(stderr, "Couldn't open log file: \"{}\"\n.", logFilePath);
    return EXIT_FAILURE;
  }

  for (int i{1}; i < argc; ++i) {
    const pl::string_view                                     filePath{argv[i]};
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

    fmt::print(ofs, "File: \"{}\"\n", filePath);

    const cl::DataSet& dataSet{expectedDataSet.value()};

    constexpr long double            accelerometerThreshold{1.99L};
    constexpr long double            gyroscopeThreshold{1999.99L};
    const std::vector<cl::DataPoint> aboveThresholdDataPoints{
      ctg::aboveThreshold(dataSet, accelerometerThreshold, gyroscopeThreshold)};

    constexpr std::size_t channelCount{cl::channelCount};
    const std::size_t     dataPointCount{channelCount * dataSet.rowCount()};
    fmt::print(
      "\"{}\": {} of {} data points ({}%) are above / below the threshold.\n",
      filePath,
      aboveThresholdDataPoints.size(),
      dataPointCount,
      ctg::percentageOf(aboveThresholdDataPoints.size(), dataPointCount));

    for (const cl::DataPoint& dataPoint : aboveThresholdDataPoints) {
      fmt::print(ofs, "{}\n", dataPoint);
    }

    fmt::print(ofs, "\n");
  }

  return EXIT_SUCCESS;
}
