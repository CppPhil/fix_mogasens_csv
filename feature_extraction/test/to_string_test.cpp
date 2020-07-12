#include "gtest/gtest.h"

#include "channel.hpp"
#include "data_point.hpp"
#include "error.hpp"
#include "sensor.hpp"
#include "to_string.hpp"

TEST(to_string, test)
{
  const fe::Channel   channel{fe::Channel::GyroscopeY};
  const fe::DataPoint dataPoint{
    /* time */ 50.5,
    /* sensor */ fe::Sensor::LeftArm,
    /* channel */ fe::Channel::AccelerometerZ,
    /* value */ 25.1123};
  const fe::Error error{
    /* kind */ fe::Error::Filesystem,
    /* file */ "test_file.cpp",
    /* function */ "a_function",
    /* line */ 5,
    /* message */ "Error message."};
  const fe::Sensor sensor{fe::Sensor::RightArm};

  EXPECT_EQ("GyroscopeY", fe::to_string(channel));
  EXPECT_EQ(
    R"(DataPoint{"time": 50.5, "sensor": "LeftArm", "channel": "AccelerometerZ", "value": 25.1123})",
    fe::to_string(dataPoint));
  EXPECT_EQ(
    R"(Error{"kind": "Filesystem", "file": "test_file.cpp", "function": "a_function", "line": 5, "message": "Error message."})",
    fe::to_string(error));
  EXPECT_EQ("RightArm", fe::to_string(sensor));
}
