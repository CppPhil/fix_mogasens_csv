#include <cassert>

#include <ostream>

#include "sensor.hpp"

namespace cl {
std::ostream& operator<<(std::ostream& os, Sensor sensor)
{
  switch (sensor) {
#define CL_SENSOR_X(enumerator, value) \
  case Sensor::enumerator: return os << #enumerator;
    CL_SENSOR
#undef CL_SENSOR_X
  }

  assert(false && "This should not be reachable!");
  return os;
}
} // namespace cl
