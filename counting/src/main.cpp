#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <array>
#include <fstream>
#include <sstream>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/string_view.hpp>

#include "cl/channel.hpp"
#include "cl/data_set.hpp"
#include "cl/read_csv_file.hpp"

#include "average_comparison_value_calculator.hpp"
#include "half_maximum_comparison_value_calculator.hpp"
#include "is_relevant.hpp"
#include "run_above_threshold.hpp"

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

  const pl::string_view aboveThresholdLogFilePath{"above_threshold_log.txt"};
  std::ofstream         aboveThresholdLogOuputFileStream{
    aboveThresholdLogFilePath.c_str(),
    std::ios_base::out | std::ios_base::trunc};

  if (!aboveThresholdLogOuputFileStream) {
    fmt::print(
      stderr, "Couldn't open log file: \"{}\"\n.", aboveThresholdLogFilePath);
    return EXIT_FAILURE;
  }

  std::ostringstream averageRelevanceOutputBuffer{};
  std::ostringstream halfMaximumRelevanceOutputBuffer{};

  fmt::print(averageRelevanceOutputBuffer, "irrelevance (average)\n");
  fmt::print(halfMaximumRelevanceOutputBuffer, "irrelevance (half maximum)\n");

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

    const cl::DataSet& dataSet{expectedDataSet.value()};

    ctg::runAboveThreshold(aboveThresholdLogOuputFileStream, dataSet);

    constexpr std::array<cl::Channel, cl::channelCount> channels{{
#define CL_CHANNEL_X(enm, v, a) ::cl::Channel::enm,
      CL_CHANNEL
#undef CL_CHANNEL_X
    }};

    for (cl::Channel channel : channels) {
      if (!ctg::isRelevant(
            channel, dataSet, &ctg::averageComparisonValueCalculator)) {
        fmt::print(
          averageRelevanceOutputBuffer,
          "{}: channel {} is not relevant.\n",
          dataSet.fileName(),
          channel);
      }
    }

    for (cl::Channel channel : channels) {
      if (!ctg::isRelevant(
            channel, dataSet, &ctg::halfMaximumComparisonValueCalculator)) {
        fmt::print(
          halfMaximumRelevanceOutputBuffer,
          "{}: channel {} isn't relevant.\n",
          dataSet.fileName(),
          channel);
      }
    }
  }

  fmt::print(
    "\n{}\n{}",
    averageRelevanceOutputBuffer.str(),
    halfMaximumRelevanceOutputBuffer.str());

  return EXIT_SUCCESS;
}
