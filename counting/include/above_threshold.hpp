#ifndef INCG_CTG_ABOVE_THRESHOLD_HPP
#define INCG_CTG_ABOVE_THRESHOLD_HPP
#include <vector>

#include "data_point.hpp"
#include "data_set.hpp"

namespace ctg {
std::vector<cl::DataPoint> aboveThreshold(
  const cl::DataSet& dataSet,
  long double        accelerometerThreshold,
  long double        gyroscopeThreshold);
} // namespace ctg
#endif // INCG_CTG_ABOVE_THRESHOLD_HPP
