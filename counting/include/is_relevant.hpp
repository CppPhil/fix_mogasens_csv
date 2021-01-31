#ifndef INCG_CTG_IS_RELEVANT_HPP
#define INCG_CTG_IS_RELEVANT_HPP
#include <cmath>

#include "cl/channel.hpp"
#include "cl/data_set.hpp"
#include "cl/sensor.hpp"

namespace ctg {
/*!
 * \brief Checks if a channel is relevant for a sensor in a data set.
 * \tparam ComparisonValueCalculator The type of the ComparisonValueCalculator
 *                                   function to use.
 * \param sensor The sensor.
 * \param channel The channel.
 * \param dataSet The data set.
 * \param comparisonValueCalculator The function to use to get the value to
 *                                  compare to.
 **/
template<typename ComparisonValueCalculator>
bool isRelevant(
  cl::Sensor                sensor,
  cl::Channel               channel,
  const cl::DataSet&        dataSet,
  ComparisonValueCalculator comparisonValueCalculator)
{
  const cl::DataSet::ChannelAccessor accessor{cl::dataSetAccessor(channel)};

  const long double comparisonValue{
    comparisonValueCalculator(sensor, channel, dataSet)};

  for (cl::DataSet::size_type i{0}; i < dataSet.rowCount(); ++i) {
    if (dataSet.extractId(i) != sensor) { continue; }

    if (std::abs((dataSet.*accessor)(i)) > comparisonValue) { return true; }
  }

  return false;
}
} // namespace ctg
#endif // INCG_CTG_IS_RELEVANT_HPP
