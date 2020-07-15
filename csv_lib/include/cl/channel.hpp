#ifndef INCG_CL_CHANNEL_HPP
#define INCG_CL_CHANNEL_HPP
#include <cstddef>
#include <cstdint>

#include <iosfwd>

#include "data_set.hpp"

#define CL_CHANNEL                                                \
  CL_CHANNEL_X(AccelerometerX, 1, &::cl::DataSet::accelerometerX) \
  CL_CHANNEL_X(AccelerometerY, 2, &::cl::DataSet::accelerometerY) \
  CL_CHANNEL_X(AccelerometerZ, 3, &::cl::DataSet::accelerometerZ) \
  CL_CHANNEL_X(GyroscopeX, 4, &::cl::DataSet::gyroscopeX)         \
  CL_CHANNEL_X(GyroscopeY, 5, &::cl::DataSet::gyroscopeY)         \
  CL_CHANNEL_X(GyroscopeZ, 6, &::cl::DataSet::gyroscopeZ)

namespace cl {
enum class Channel : std::uint64_t {
#define CL_CHANNEL_X(enumerator, value, dataSetAccessor) enumerator = value,
  CL_CHANNEL
#undef CL_CHANNEL_X
};

inline constexpr std::size_t channelCount{0
#define CL_CHANNEL_X(enumerator, value, dataSetAccessor) +1
                                          CL_CHANNEL
#undef CL_CHANNEL_X
};

template<Channel Chan>
struct data_set_accessor;

#define CL_CHANNEL_X(enumerator, value, dataSetAccessor)                 \
  template<>                                                             \
  struct data_set_accessor<Channel::enumerator> {                        \
    static constexpr ::cl::DataSet::ChannelAccessor f = dataSetAccessor; \
  };
CL_CHANNEL
#undef CL_CHANNEL_X

template<Channel Chan>
inline constexpr DataSet::ChannelAccessor data_set_accessor_v
  = data_set_accessor<Chan>::f;

DataSet::ChannelAccessor dataSetAccessor(Channel channel);

std::ostream& operator<<(std::ostream& os, Channel channel);

bool isAccelerometer(Channel channel);

bool isGyroscope(Channel channel);
} // namespace cl
#endif // INCG_CL_CHANNEL_HPP