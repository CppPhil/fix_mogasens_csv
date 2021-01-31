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

/*!
 * \brief Scoped enum for the sensors.
 **/
enum class Sensor : std::uint64_t {
#define CL_SENSOR_X(enumerator, value) enumerator = value,
  CL_SENSOR
#undef CL_SENSOR_X
};

/*!
 * \brief The sensors
 **/
inline constexpr std::array<Sensor, 4> sensors{{
#define CL_SENSOR_X(enm, v) ::cl::Sensor::enm,
  CL_SENSOR
#undef CL_SENSOR_X
}};

/*!
 * \brief Prints a sensor to an ostream.
 * \param os The ostream to print to.
 * \param sensor The sensor to print.
 * \return `os`.
 **/
std::ostream& operator<<(std::ostream& os, Sensor sensor);
} // namespace cl
#endif // INCG_CL_SENSOR_HPP
