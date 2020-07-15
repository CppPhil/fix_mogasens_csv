#include <cinttypes>

#include <array>
#include <utility>

#include <tl/casts.hpp>

#include <pl/numeric.hpp>

#include "above_threshold.hpp"

namespace ctg {
namespace {
using size_type = cl::DataSet::size_type;

struct ChannelTableEntry {
  cl::DataSet::ChannelAccessor channelAccessor;
  cl::Channel                  channel;
};

constexpr std::array<ChannelTableEntry, cl::channelCount> channelTable{{
#define CL_CHANNEL_X(enm, v, accessor) {accessor, cl::Channel::enm},
  CL_CHANNEL
#undef CL_CHANNEL_X
}};
} // namespace

std::vector<cl::DataPoint> aboveThreshold(
  const cl::DataSet& dataSet,
  long double        accelerometerThreshold,
  long double        gyroscopeThreshold)
{
  std::vector<cl::DataPoint> result{};

  const auto isAboveThreshold = [](long double threshold, long double value) {
    return std::make_pair((value > threshold) || (value < -threshold), value);
  };

  for (size_type i{0}; i < dataSet.rowCount(); ++i) {
    const auto       currentTime{dataSet.time(i)};
    const cl::Sensor currentSensor{dataSet.extractId(i)};

    for (const auto [channelAccessor, currentChannel] : channelTable) {
      if (const auto [isAbove, channelValue] = isAboveThreshold(
            (cl::isAccelerometer(currentChannel)) ? accelerometerThreshold
                                                  : gyroscopeThreshold,
            (dataSet.*channelAccessor)(i));
          isAbove) {
        result.emplace_back(
          dataSet.fileName(),
          currentTime,
          currentSensor,
          currentChannel,
          channelValue);
      }
    }
  }

  return result;
}
} // namespace ctg
