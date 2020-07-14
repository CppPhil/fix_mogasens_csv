#include <sstream>

#include "gtest/gtest.h"

#include "sensor.hpp"

TEST(sensor, shouldHaveCorrectValues)
{
  EXPECT_EQ(769, static_cast<std::uint64_t>(fe::Sensor::LeftArm));
  EXPECT_EQ(770, static_cast<std::uint64_t>(fe::Sensor::Belly));
  EXPECT_EQ(771, static_cast<std::uint64_t>(fe::Sensor::RightArm));
  EXPECT_EQ(772, static_cast<std::uint64_t>(fe::Sensor::Chest));
}

TEST(sensor, shouldPrintCorretly)
{
  const auto s = [](fe::Sensor sensor) {
    std::ostringstream oss{};
    oss << sensor;
    return oss.str();
  };

  EXPECT_EQ("RightArm", s(fe::Sensor::RightArm));
  EXPECT_EQ("Belly", s(fe::Sensor::Belly));
  EXPECT_EQ("Chest", s(fe::Sensor::Chest));
  EXPECT_EQ("LeftArm", s(fe::Sensor::LeftArm));
}
