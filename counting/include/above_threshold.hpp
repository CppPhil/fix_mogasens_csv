#ifndef INCG_CTG_ABOVE_THRESHOLD_HPP
#define INCG_CTG_ABOVE_THRESHOLD_HPP
#include <vector>

#include "cl/data_point.hpp"
#include "cl/data_set.hpp"

namespace ctg {
/*!
 * \brief Returns all the data points that are above / below the threshold.
 * \param dataSet The data set.
 * \param accelerometerThreshold The accelerometer threshold (must be positive)
 * \param gyroscopeThreshold The gyroscope threshold (must be positive)
 * \return The data points that are invalid.
 * \note The unary operator operator- is applied to the two thresholds given
 *       to determine the minimum threshold.
 **/
std::vector<cl::DataPoint> aboveThreshold(
  const cl::DataSet& dataSet,
  long double        accelerometerThreshold,
  long double        gyroscopeThreshold);
} // namespace ctg
#endif // INCG_CTG_ABOVE_THRESHOLD_HPP
