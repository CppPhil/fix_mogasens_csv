#include <cassert>

#include <ostream>

#include "sensor.hpp"

namespace fe {
std::ostream& operator<<(std::ostream& os, Sensor sensor)
{
  switch (sensor) {
#define FE_SENSOR_X(enumerator, value) \
  case Sensor::enumerator: return os << #enumerator;
    FE_SENSOR
#undef FE_SENSOR_X
  }

  assert(false && "This should not be reachable!");
  return os;
}
} // namespace fe
