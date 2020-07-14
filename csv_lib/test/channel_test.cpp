#include <sstream>

#include "gtest/gtest.h"

#include "channel.hpp"

TEST(channel, shouldHaveCorrectValues)
{
  EXPECT_EQ(1, static_cast<std::uint64_t>(cl::Channel::AccelerometerX));
  EXPECT_EQ(2, static_cast<std::uint64_t>(cl::Channel::AccelerometerY));
  EXPECT_EQ(3, static_cast<std::uint64_t>(cl::Channel::AccelerometerZ));
  EXPECT_EQ(4, static_cast<std::uint64_t>(cl::Channel::GyroscopeX));
  EXPECT_EQ(5, static_cast<std::uint64_t>(cl::Channel::GyroscopeY));
  EXPECT_EQ(6, static_cast<std::uint64_t>(cl::Channel::GyroscopeZ));
}

TEST(channel, shouldPrintCorrectly)
{
  const auto s = [](cl::Channel c) {
    std::ostringstream oss{};
    oss << c;
    return oss.str();
  };

  EXPECT_EQ("AccelerometerX", s(cl::Channel::AccelerometerX));
  EXPECT_EQ("AccelerometerY", s(cl::Channel::AccelerometerY));
  EXPECT_EQ("AccelerometerZ", s(cl::Channel::AccelerometerZ));
  EXPECT_EQ("GyroscopeX", s(cl::Channel::GyroscopeX));
  EXPECT_EQ("GyroscopeY", s(cl::Channel::GyroscopeY));
  EXPECT_EQ("GyroscopeZ", s(cl::Channel::GyroscopeZ));
}
