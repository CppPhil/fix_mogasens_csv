#ifndef INCG_CM_IMU_HPP
#define INCG_CM_IMU_HPP
#include <cstddef>

#include <array>

#include <iosfwd>

namespace cm {
#define CM_IMU            \
  CM_IMU_X(Accelerometer) \
  CM_IMU_X(Gyroscope)

/*!
 * \brief Scoped enum type for the IMUs.
 **/
enum class Imu {
#define CM_IMU_X(enm) enm,
  CM_IMU
#undef CM_IMU_X
};

/*!
 * \brief The amount of IMUs.
 **/
inline constexpr std::size_t imuCount{0
#define CM_IMU_X(enm) +1
                                      CM_IMU
#undef CM_IMU_X
};

/*!
 * \brief An array of the IMU enumerators.
 **/
inline constexpr std::array<Imu, imuCount> imus{{
#define CM_IMU_X(enm) ::cm::Imu::enm,
  CM_IMU
#undef CM_IMU_X
}};

/*!
 * \brief Prints `imu` to `os`.
 * \param os The ostream to print to
 * \param imu The IMU enumerator to print.
 * \return `os`
 **/
std::ostream& operator<<(std::ostream& os, Imu imu);
} // namespace cm
#endif // INCG_CM_IMU_HPP
