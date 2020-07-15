#ifndef INCG_CTG_IS_RELEVANT_HPP
#define INCG_CTG_IS_RELEVANT_HPP
#include <cmath>

#include "cl/channel.hpp"
#include "cl/data_set.hpp"
#include "cl/sensor.hpp"

namespace ctg {
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
