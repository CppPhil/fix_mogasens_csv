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
  long double (cl::DataSet::*channelAccessor)(size_type) const;
  cl::Channel channel;
};

constexpr std::size_t channelCount{cl::channelCount};

constexpr std::array<ChannelTableEntry, channelCount> channelTable{
  {{&cl::DataSet::accelerometerX, cl::Channel::AccelerometerX},
   {&cl::DataSet::accelerometerY, cl::Channel::AccelerometerY},
   {&cl::DataSet::accelerometerZ, cl::Channel::AccelerometerZ},
   {&cl::DataSet::gyroscopeX, cl::Channel::GyroscopeX},
   {&cl::DataSet::gyroscopeY, cl::Channel::GyroscopeY},
   {&cl::DataSet::gyroscopeZ, cl::Channel::GyroscopeZ}}};
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
