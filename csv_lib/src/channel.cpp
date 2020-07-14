#include <cassert>

#include <ostream>

#include "channel.hpp"

namespace fe {
std::ostream& operator<<(std::ostream& os, Channel channel)
{
  switch (channel) {
#define FE_CHANNEL_X(enumerator, value) \
  case Channel::enumerator: return os << #enumerator;
    FE_CHANNEL
#undef FE_CHANNEL_X
  }

  assert(false && "This should not be reachable!");
  return os;
}
} // namespace fe
