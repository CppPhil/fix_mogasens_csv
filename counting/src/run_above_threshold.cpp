#include <ostream>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "cl/channel.hpp"
#include "cl/data_point.hpp"

#include "above_threshold.hpp"
#include "percentage_of.hpp"
#include "run_above_threshold.hpp"

namespace ctg {
void runAboveThreshold(
  std::ostream&      aboveThresholdLogOutputFileStream,
  const cl::DataSet& dataSet)
{
  fmt::print(
    aboveThresholdLogOutputFileStream, "File: \"{}\"\n", dataSet.fileName());

  const std::vector<cl::DataPoint> aboveThresholdDataPoints{ctg::aboveThreshold(
    dataSet, cl::accelerometerThreshold, cl::gyroscopeThreshold)};

  const std::size_t dataPointCount{cl::channelCount * dataSet.rowCount()};

  fmt::print(
    "{}: {} of {} data points ({}%) are above / below the threshold.\n",
    dataSet.fileName(),
    aboveThresholdDataPoints.size(),
    dataPointCount,
    ctg::percentageOf(aboveThresholdDataPoints.size(), dataPointCount));

  for (const cl::DataPoint& dataPoint : aboveThresholdDataPoints) {
    fmt::print(aboveThresholdLogOutputFileStream, "{}\n", dataPoint);
  }

  fmt::print(aboveThresholdLogOutputFileStream, "\n");
}
} // namespace ctg
