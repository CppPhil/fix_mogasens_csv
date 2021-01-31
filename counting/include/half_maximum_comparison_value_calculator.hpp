#ifndef INCG_CTG_HALF_MAXIMUM_COMPARISON_VALUE_CALCULATOR_HPP
#define INCG_CTG_HALF_MAXIMUM_COMPARISON_VALUE_CALCULATOR_HPP
#include "cl/channel.hpp"
#include "cl/data_set.hpp"
#include "cl/sensor.hpp"

namespace ctg {
/*!
 * \brief Returns half of the maximum for the given sensor, channel and data
 *        set.
 * \param sensor The sensor.
 * \param channel The channel.
 * \param dataSet The data set.
 * \return Half of the maximum.
 **/
long double halfMaximumComparisonValueCalculator(
  cl::Sensor         sensor,
  cl::Channel        channel,
  const cl::DataSet& dataSet);
} // namespace ctg
#endif // INCG_CTG_HALF_MAXIMUM_COMPARISON_VALUE_CALCULATOR_HPP
