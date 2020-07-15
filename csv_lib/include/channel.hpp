#ifndef INCG_CL_CHANNEL_HPP
#define INCG_CL_CHANNEL_HPP
#include <cstdint>

#include <iosfwd>

#define CL_CHANNEL                \
  CL_CHANNEL_X(AccelerometerX, 1) \
  CL_CHANNEL_X(AccelerometerY, 2) \
  CL_CHANNEL_X(AccelerometerZ, 3) \
  CL_CHANNEL_X(GyroscopeX, 4)     \
  CL_CHANNEL_X(GyroscopeY, 5)     \
  CL_CHANNEL_X(GyroscopeZ, 6)

namespace cl {
enum class Channel : std::uint64_t {
#define CL_CHANNEL_X(enumerator, value) enumerator = value,
  CL_CHANNEL
#undef CL_CHANNEL_X
};

std::ostream& operator<<(std::ostream& os, Channel channel);

bool isAccelerometer(Channel channel);

bool isGyroscope(Channel channel);
} // namespace cl
#endif // INCG_CL_CHANNEL_HPP