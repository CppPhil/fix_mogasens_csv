#ifndef INCG_CTG_AVERAGE_COMPARISON_VALUE_CALCULATOR_HPP
#define INCG_CTG_AVERAGE_COMPARISON_VALUE_CALCULATOR_HPP
#include "cl/channel.hpp"
#include "cl/data_set.hpp"
#include "cl/sensor.hpp"

namespace ctg {
/*!
 * \brief Calculates the average value for the given sensor, channel and data
 *        set.
 * \param sensor The sensor.
 * \param channel The channel.
 * \param dataSet The data set.
 * \return The average value.
 **/
long double averageComparisonValueCalculator(
  cl::Sensor         sensor,
  cl::Channel        channel,
  const cl::DataSet& dataSet);
} // namespace ctg
#endif // INCG_CTG_AVERAGE_COMPARISON_VALUE_CALCULATOR_HPP
