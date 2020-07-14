#include <sstream>

#include "gtest/gtest.h"

#include "data_point.hpp"

const cl::DataPoint dp{
  /* fileName */ "file.csv",
  /* time */ 0.01,
  /* sensor */ cl::Sensor::Chest,
  /* channel */ cl::Channel::AccelerometerX,
  /* value */ 50.01};

TEST(DataPoint, shouldPrintCorrectly)
{
  std::ostringstream oss{};
  oss << dp;
  EXPECT_EQ(
    R"(DataPoint{"time": 0.01, "sensor": "Chest", "channel": "AccelerometerX", "value": 50.01})",
    oss.str());
}

TEST(DataPoint, shouldGetValuesCorrectly)
{
  EXPECT_EQ("file.csv", dp.fileName());
  EXPECT_EQ(0.01, dp.time());
  EXPECT_EQ(cl::Sensor::Chest, dp.sensor());
  EXPECT_EQ(cl::Channel::AccelerometerX, dp.channel());
  EXPECT_EQ(50.01, dp.value());
}
