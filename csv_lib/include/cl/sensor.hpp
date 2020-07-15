#ifndef INCG_CL_SENSOR_HPP
#define INCG_CL_SENSOR_HPP
#include <cstdint>

#include <array>
#include <iosfwd>

namespace cl {
#define CL_SENSOR            \
  CL_SENSOR_X(LeftArm, 769)  \
  CL_SENSOR_X(Belly, 770)    \
  CL_SENSOR_X(RightArm, 771) \
  CL_SENSOR_X(Chest, 772)

enum class Sensor : std::uint64_t {
#define CL_SENSOR_X(enumerator, value) enumerator = value,
  CL_SENSOR
#undef CL_SENSOR_X
};

inline constexpr std::array<Sensor, 4> sensors{{
#define CL_SENSOR_X(enm, v) ::cl::Sensor::enm,
  CL_SENSOR
#undef CL_SENSOR_X
}};

std::ostream& operator<<(std::ostream& os, Sensor sensor);
} // namespace cl
#endif // INCG_CL_SENSOR_HPP
