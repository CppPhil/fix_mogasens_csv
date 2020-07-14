#ifndef INCG_CL_DATA_POINT_HPP
#define INCG_CL_DATA_POINT_HPP
#include <iosfwd>
#include <string>

#include "channel.hpp"
#include "sensor.hpp"

namespace cl {
class DataPoint {
public:
  friend std::ostream& operator<<(std::ostream& os, const DataPoint& dataPoint);

  DataPoint(
    std::string fileName,
    long double time,
    Sensor      sensor,
    Channel     channel,
    long double value) noexcept;

  const std::string& fileName() const noexcept;

  long double time() const noexcept;

  Sensor sensor() const noexcept;

  Channel channel() const noexcept;

  long double value() const noexcept;

private:
  std::string m_fileName;
  long double m_time;
  Sensor      m_sensor;
  Channel     m_channel;
  long double m_value;
};
} // namespace cl
#endif // INCG_CL_DATA_POINT_HPP
