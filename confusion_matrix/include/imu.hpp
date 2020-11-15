#ifndef INCG_CM_IMU_HPP
#define INCG_CM_IMU_HPP
#include <cstddef>

#include <array>

#include <iosfwd>

namespace cm {
#define CM_IMU            \
  CM_IMU_X(Accelerometer) \
  CM_IMU_X(Gyroscope)

enum class Imu {
#define CM_IMU_X(enm) enm,
  CM_IMU
#undef CM_IMU_X
};

inline constexpr std::size_t imuCount{0
#define CM_IMU_X(enm) +1
                                      CM_IMU
#undef CM_IMU_X
};

inline constexpr std::array<Imu, imuCount> imus{{
#define CM_IMU_X(enm) ::cm::Imu::enm,
  CM_IMU
#undef CM_IMU_X
}};

std::ostream& operator<<(std::ostream& os, Imu imu);
} // namespace cm
#endif // INCG_CM_IMU_HPP
