#include <sstream>

#include "gtest/gtest.h"

#include "channel.hpp"

TEST(channel, shouldHaveCorrectValues)
{
  EXPECT_EQ(1, static_cast<std::uint64_t>(fe::Channel::AccelerometerX));
  EXPECT_EQ(2, static_cast<std::uint64_t>(fe::Channel::AccelerometerY));
  EXPECT_EQ(3, static_cast<std::uint64_t>(fe::Channel::AccelerometerZ));
  EXPECT_EQ(4, static_cast<std::uint64_t>(fe::Channel::GyroscopeX));
  EXPECT_EQ(5, static_cast<std::uint64_t>(fe::Channel::GyroscopeY));
  EXPECT_EQ(6, static_cast<std::uint64_t>(fe::Channel::GyroscopeZ));
}

TEST(channel, shouldPrintCorrectly)
{
  const auto s = [](fe::Channel c) {
    std::ostringstream oss{};
    oss << c;
    return oss.str();
  };

  EXPECT_EQ("AccelerometerX", s(fe::Channel::AccelerometerX));
  EXPECT_EQ("AccelerometerY", s(fe::Channel::AccelerometerY));
  EXPECT_EQ("AccelerometerZ", s(fe::Channel::AccelerometerZ));
  EXPECT_EQ("GyroscopeX", s(fe::Channel::GyroscopeX));
  EXPECT_EQ("GyroscopeY", s(fe::Channel::GyroscopeY));
  EXPECT_EQ("GyroscopeZ", s(fe::Channel::GyroscopeZ));
}
