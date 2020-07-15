#include <cassert>
#include <cstdio>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "average_comparison_value_calculator.hpp"

namespace ctg {
long double averageComparisonValueCalculator(
  cl::Sensor         sensor,
  cl::Channel        channel,
  const cl::DataSet& dataSet)
{
  if (cl::isAccelerometer(channel)) {
    return dataSet.accelerometerAverage(sensor);
  }

  if (cl::isGyroscope(channel)) { return dataSet.gyroscopeAverage(sensor); }

  fmt::print(stderr, "channel was neither accelerometer nor gyroscope!\n");
  assert(false && "channel was neither accelerometer nor gyroscope!");
  return 0.0L;
}
} // namespace ctg
