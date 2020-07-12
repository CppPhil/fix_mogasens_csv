#ifndef INCG_FE_CHANNEL_HPP
#define INCG_FE_CHANNEL_HPP
#include <cstdint>

#include <iosfwd>

#define FE_CHANNEL                \
  FE_CHANNEL_X(AccelerometerX, 1) \
  FE_CHANNEL_X(AccelerometerY, 2) \
  FE_CHANNEL_X(AccelerometerZ, 3) \
  FE_CHANNEL_X(GyroscopeX, 4)     \
  FE_CHANNEL_X(GyroscopeY, 5)     \
  FE_CHANNEL_X(GyroscopeZ, 6)

namespace fe {
enum class Channel : std::uint64_t {
#define FE_CHANNEL_X(enumerator, value) enumerator = value,
  FE_CHANNEL
#undef FE_CHANNEL_X
};

std::ostream& operator<<(std::ostream& os, Channel channel);
} // namespace fe
#endif // INCG_FE_CHANNEL_HPP