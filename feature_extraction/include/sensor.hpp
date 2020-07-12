#ifndef INCG_FE_SENSOR_HPP
#define INCG_FE_SENSOR_HPP
#include <cstdint>

#include <iosfwd>

namespace fe {
#define FE_SENSOR            \
  FE_SENSOR_X(LeftArm, 769)  \
  FE_SENSOR_X(Belly, 770)    \
  FE_SENSOR_X(RightArm, 771) \
  FE_SENSOR_X(Chest, 772)

enum class Sensor : std::uint64_t {
#define FE_SENSOR_X(enumerator, value) enumerator = value,
  FE_SENSOR
#undef FE_SENSOR_X
};

std::ostream& operator<<(std::ostream& os, Sensor sensor);
} // namespace fe
#endif // INCG_FE_SENSOR_HPP
