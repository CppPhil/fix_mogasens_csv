#include <cassert>
#include <cinttypes>
#include <cstring>

#include <ostream>

#include <tl/casts.hpp>

#include <pl/numeric.hpp>

#include "cl/channel.hpp"

namespace cl {
DataSet::ChannelAccessor dataSetAccessor(Channel channel)
{
  switch (channel) {
#define CL_CHANNEL_X(enm, v, acc) \
  case Channel::enm: return data_set_accessor_v<Channel::enm>;
    CL_CHANNEL
#undef CL_CHANNEL_X
  }

  assert(
    false
    && "channel enumerator has invalid value in channel.cpp: dataSetAccessor!");
  DataSet::ChannelAccessor r;
  std::memset(&r, 0, sizeof(r));
  return r;
}

std::ostream& operator<<(std::ostream& os, Channel channel)
{
  switch (channel) {
#define CL_CHANNEL_X(enumerator, value, dataSetAccessor) \
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
