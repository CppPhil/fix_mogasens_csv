#include <sstream>

#include "gtest/gtest.h"

#include "sensor.hpp"

TEST(sensor, shouldHaveCorrectValues)
{
  EXPECT_EQ(769, static_cast<std::uint64_t>(cl::Sensor::LeftArm));
  EXPECT_EQ(770, static_cast<std::uint64_t>(cl::Sensor::Belly));
  EXPECT_EQ(771, static_cast<std::uint64_t>(cl::Sensor::RightArm));
  EXPECT_EQ(772, static_cast<std::uint64_t>(cl::Sensor::Chest));
}

TEST(sensor, shouldPrintCorretly)
{
  const auto s = [](cl::Sensor sensor) {
    std::ostringstream oss{};
    oss << sensor;
    return oss.str();
  };

  EXPECT_EQ("RightArm", s(cl::Sensor::RightArm));
  EXPECT_EQ("Belly", s(cl::Sensor::Belly));
  EXPECT_EQ("Chest", s(cl::Sensor::Chest));
  EXPECT_EQ("LeftArm", s(cl::Sensor::LeftArm));
}
