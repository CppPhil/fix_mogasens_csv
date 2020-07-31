#include <ostream>
#include <utility>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "cl/data_point.hpp"

namespace cl {
std::ostream& operator<<(std::ostream& os, const DataPoint& dataPoint)
{
  return os << fmt::format(
           R"(DataPoint{{"fileName": "{}", time": {}, "sensor": "{}", "channel": "{}", "value": {}}})",
           dataPoint.fileName(),
           dataPoint.time(),
           dataPoint.sensor(),
           dataPoint.channel(),
           dataPoint.value());
}

DataPoint::DataPoint(
  std::string fileName,
  long double time,
  Sensor      sensor,
  Channel     channel,
  long double value) noexcept
  : m_fileName{std::move(fileName)}
  , m_time{time}
  , m_sensor{sensor}
  , m_channel{channel}
  , m_value{value}
{
}

const std::string& DataPoint::fileName() const noexcept { return m_fileName; }

long double DataPoint::time() const noexcept { return m_time; }

Sensor DataPoint::sensor() const noexcept { return m_sensor; }

Channel DataPoint::channel() const noexcept { return m_channel; }

long double DataPoint::value() const noexcept { return m_value; }
} // namespace cl
