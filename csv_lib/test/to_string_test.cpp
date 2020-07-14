#include "gtest/gtest.h"

#include "channel.hpp"
#include "data_point.hpp"
#include "error.hpp"
#include "sensor.hpp"
#include "to_string.hpp"

TEST(to_string, test)
{
  const cl::Channel   channel{cl::Channel::GyroscopeY};
  const cl::DataPoint dataPoint{
    /* time */ 50.5,
    /* sensor */ cl::Sensor::LeftArm,
    /* channel */ cl::Channel::AccelerometerZ,
    /* value */ 25.1123};
  const cl::Error error{
    /* kind */ cl::Error::Filesystem,
    /* file */ "test_file.cpp",
    /* function */ "a_function",
    /* line */ 5,
    /* message */ "Error message."};
  const cl::Sensor sensor{cl::Sensor::RightArm};

  EXPECT_EQ("GyroscopeY", cl::to_string(channel));
  EXPECT_EQ(
    R"(DataPoint{"time": 50.5, "sensor": "LeftArm", "channel": "AccelerometerZ", "value": 25.1123})",
    cl::to_string(dataPoint));
  EXPECT_EQ(
    R"(Error{"kind": "Filesystem", "file": "test_file.cpp", "function": "a_function", "line": 5, "message": "Error message."})",
    cl::to_string(error));
  EXPECT_EQ("RightArm", cl::to_string(sensor));
}
