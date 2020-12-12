#ifndef INCG_CL_CHANNEL_HPP
#define INCG_CL_CHANNEL_HPP
#include <cstddef>
#include <cstdint>

#include <array>
#include <iosfwd>

#include "cl/data_set.hpp"

#define CL_CHANNEL                                                \
  CL_CHANNEL_X(AccelerometerX, 1, &::cl::DataSet::accelerometerX) \
  CL_CHANNEL_X(AccelerometerY, 2, &::cl::DataSet::accelerometerY) \
  CL_CHANNEL_X(AccelerometerZ, 3, &::cl::DataSet::accelerometerZ) \
  CL_CHANNEL_X(GyroscopeX, 4, &::cl::DataSet::gyroscopeX)         \
  CL_CHANNEL_X(GyroscopeY, 5, &::cl::DataSet::gyroscopeY)         \
  CL_CHANNEL_X(GyroscopeZ, 6, &::cl::DataSet::gyroscopeZ)

namespace cl {
/*!
 * \brief Scoped enum for the 6 sensor channels.
 **/
enum class Channel : std::uint64_t {
#define CL_CHANNEL_X(enumerator, value, dataSetAccessor) enumerator = value,
  CL_CHANNEL
#undef CL_CHANNEL_X
};

/*!
 * \brief The amount of sensor channels (6).
 **/
inline constexpr std::size_t channelCount{0
#define CL_CHANNEL_X(enumerator, value, dataSetAccessor) +1
                                          CL_CHANNEL
#undef CL_CHANNEL_X
};

/*!
 * \brief An array of the sensor channel enumerators.
 **/
inline constexpr std::array<Channel, channelCount> channels{{
#define CL_CHANNEL_X(enm, v, a) ::cl::Channel::enm,
  CL_CHANNEL
#undef CL_CHANNEL_X
}};

/*!
 * \brief Maps a `Channel` to its associated accessor member function pointer of
 *        the `DataSet` type.
 * \tparam Chan The `Channel` to map to its data set accessor.
 **/
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

/*!
 * \brief Returns the data set accessor for the `Channel` given.
 * \param channel The `Channel` to get the data set accessor for.
 * \return The data set accessor for `channel`.
 **/
DataSet::ChannelAccessor dataSetAccessor(Channel channel);

/*!
 * \brief Prints a given `Channel` to `os`.
 * \param os The ostream to print to.
 * \param channel The `Channel` to print.
 * \return `os`.
 **/
std::ostream& operator<<(std::ostream& os, Channel channel);

/*!
 * \brief Checks whether a given `Channel` is an accelerometer channel.
 * \param channel The `Channel` to check.
 * \return true if `channel` is an accelerometer sensor channel; false
 *         otherwise.
 **/
[[nodiscard]] bool isAccelerometer(Channel channel);

/*!
 * \brief Checks whether a given `Channel` is a gyroscope channel.
 * \param channel The `Channel` to check.
 * \return true if `channel` is a gyroscope sensor channel; false
 *         otherwise.
 **/
[[nodiscard]] bool isGyroscope(Channel channel);

/*!
 * \brief Constant for the maximum acceptable accelerometer value (in positive
 *        and negative direction).
 **/
inline constexpr long double accelerometerThreshold{1.99L};

/*!
 * \brief Constant for the maximum acceptable gyroscope value (in positive
 *        and negative direction).
 **/
inline constexpr long double gyroscopeThreshold{1999.99L};

/*!
 * \brief Returns the threshold value for `channel`.
 * \param channel The `Channel` to get the threshold value of.
 * \return The threshold value for `channel`.
 **/
[[nodiscard]] long double threshold(Channel channel);
} // namespace cl
#endif // INCG_CL_CHANNEL_HPP
