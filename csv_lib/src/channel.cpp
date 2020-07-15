#include <cassert>
#include <cinttypes>

#include <ostream>

#include <tl/casts.hpp>

#include <pl/numeric.hpp>

#include "channel.hpp"

namespace cl {
std::ostream& operator<<(std::ostream& os, Channel channel)
{
  switch (channel) {
#define CL_CHANNEL_X(enumerator, value) \
  case Channel::enumerator: return os << #enumerator;
    CL_CHANNEL
#undef CL_CHANNEL_X
  }

  assert(false && "This should not be reachable!");
  return os;
}

bool isAccelerometer(Channel channel)
{
  return pl::is_between(tl::underlying_cast(channel), UINT64_C(1), UINT64_C(3));
}

bool isGyroscope(Channel channel)
{
  return pl::is_between(tl::underlying_cast(channel), UINT64_C(4), UINT64_C(6));
}
} // namespace cl
