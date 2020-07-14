#ifndef INCG_FE_DATA_POINT_HPP
#define INCG_FE_DATA_POINT_HPP
#include <iosfwd>

#include "channel.hpp"
#include "sensor.hpp"

namespace fe {
class DataPoint {
public:
  friend std::ostream& operator<<(std::ostream& os, const DataPoint& dataPoint);

  DataPoint(
    long double time,
    Sensor      sensor,
    Channel     channel,
    long double value) noexcept;

  long double time() const noexcept;

  Sensor sensor() const noexcept;

  Channel channel() const noexcept;

  long double value() const noexcept;

private:
  long double m_time;
  Sensor      m_sensor;
  Channel     m_channel;
  long double m_value;
};
} // namespace fe
#endif // INCG_FE_DATA_POINT_HPP
