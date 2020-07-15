#include <cassert>
#include <cstdio>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "half_maximum_comparison_value_calculator.hpp"

namespace ctg {
long double halfMaximumComparisonValueCalculator(
  cl::Sensor         sensor,
  cl::Channel        channel,
  const cl::DataSet& dataSet)
{
  constexpr long double halve{2.0L};

  if (cl::isAccelerometer(channel)) {
    return dataSet.accelerometerMaximum(sensor) / halve;
  }

  if (cl::isGyroscope(channel)) {
    return dataSet.gyroscopeMaximum(sensor) / halve;
  }

  fmt::print(stderr, "channel was neither accelerometer nor gyroscope!\n");
  assert(false && "channel was neither accelerometer nor gyroscope!");
  return 0.0L;
}
} // namespace ctg
