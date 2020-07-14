#include <cassert>

#include <ostream>

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
} // namespace cl
