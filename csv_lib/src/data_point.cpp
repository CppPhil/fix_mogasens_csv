#include <ostream>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "data_point.hpp"

namespace fe {
std::ostream& operator<<(std::ostream& os, const DataPoint& dataPoint)
{
  return os << fmt::format(
           R"(DataPoint{{"time": {}, "sensor": "{}", "channel": "{}", "value": {}}})",
           dataPoint.time(),
           dataPoint.sensor(),
           dataPoint.channel(),
           dataPoint.value());
}

DataPoint::DataPoint(
  long double time,
  Sensor      sensor,
  Channel     channel,
  long double value) noexcept
  : m_time{time}, m_sensor{sensor}, m_channel{channel}, m_value{value}
{
}

long double DataPoint::time() const noexcept { return m_time; }

Sensor DataPoint::sensor() const noexcept { return m_sensor; }

Channel DataPoint::channel() const noexcept { return m_channel; }

long double DataPoint::value() const noexcept { return m_value; }
} // namespace fe
