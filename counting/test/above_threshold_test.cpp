#include "gtest/gtest.h"

#include "above_threshold.hpp"

#ifndef EXPECT_LONG_DOUBLE_EQ
#define EXPECT_LONG_DOUBLE_EQ(a, b) \
  EXPECT_DOUBLE_EQ(static_cast<double>(a), static_cast<double>(b))
#endif

TEST(aboveThreshold, shouldFindDataPointsIfThereAreAny)
{
  const cl::Expected<cl::DataSet> expectedDataSet{cl::DataSet::create(
    "test.csv",
    {{"0.01",
      "65100",
      "769",
      "0",
      "1.0",
      "2.0",
      "3.0",
      "500.0",
      "600.0",
      "700.0"},
     {"0.01",
      "65100",
      "770",
      "0",
      "-2.00001",
      "1.909",
      "30.0",
      "5000.0",
      "6000.0",
      "7000.0"}})};

  ASSERT_TRUE(expectedDataSet.has_value());

  const cl::DataSet& dataSet{expectedDataSet.value()};

  const std::vector<cl::DataPoint> dataPoints{
    ctg::aboveThreshold(dataSet, 1.99L, 640.2L)};

  ASSERT_EQ(8U, dataPoints.size());

  EXPECT_EQ("test.csv", dataPoints.at(0).fileName());
  EXPECT_LONG_DOUBLE_EQ(0.01L, dataPoints.at(0).time());
  EXPECT_EQ(cl::Sensor::LeftArm, dataPoints.at(0).sensor());
  EXPECT_EQ(cl::Channel::AccelerometerY, dataPoints.at(0).channel());
  EXPECT_LONG_DOUBLE_EQ(2.0L, dataPoints.at(0).value());

  EXPECT_EQ("test.csv", dataPoints.at(1).fileName());
  EXPECT_LONG_DOUBLE_EQ(0.01L, dataPoints.at(1).time());
  EXPECT_EQ(cl::Sensor::LeftArm, dataPoints.at(1).sensor());
  EXPECT_EQ(cl::Channel::AccelerometerZ, dataPoints.at(1).channel());
  EXPECT_LONG_DOUBLE_EQ(3.0L, dataPoints.at(1).value());

  EXPECT_EQ("test.csv", dataPoints.at(2).fileName());
  EXPECT_LONG_DOUBLE_EQ(0.01L, dataPoints.at(2).time());
  EXPECT_EQ(cl::Sensor::LeftArm, dataPoints.at(2).sensor());
  EXPECT_EQ(cl::Channel::GyroscopeZ, dataPoints.at(2).channel());
  EXPECT_LONG_DOUBLE_EQ(700.0L, dataPoints.at(2).value());

  EXPECT_EQ("test.csv", dataPoints.at(3).fileName());
  EXPECT_LONG_DOUBLE_EQ(0.01L, dataPoints.at(3).time());
  EXPECT_EQ(cl::Sensor::Belly, dataPoints.at(3).sensor());
  EXPECT_EQ(cl::Channel::AccelerometerX, dataPoints.at(3).channel());
  EXPECT_LONG_DOUBLE_EQ(-2.00001, dataPoints.at(3).value());

  EXPECT_EQ("test.csv", dataPoints.at(4).fileName());
  EXPECT_LONG_DOUBLE_EQ(0.01L, dataPoints.at(4).time());
  EXPECT_EQ(cl::Sensor::Belly, dataPoints.at(4).sensor());
  EXPECT_EQ(cl::Channel::AccelerometerZ, dataPoints.at(4).channel());
  EXPECT_LONG_DOUBLE_EQ(30.0L, dataPoints.at(4).value());

  EXPECT_EQ("test.csv", dataPoints.at(5).fileName());
  EXPECT_LONG_DOUBLE_EQ(0.01L, dataPoints.at(5).time());
  EXPECT_EQ(cl::Sensor::Belly, dataPoints.at(5).sensor());
  EXPECT_EQ(cl::Channel::GyroscopeX, dataPoints.at(5).channel());
  EXPECT_LONG_DOUBLE_EQ(5000.0L, dataPoints.at(5).value());

  EXPECT_EQ("test.csv", dataPoints.at(6).fileName());
  EXPECT_LONG_DOUBLE_EQ(0.01L, dataPoints.at(6).time());
  EXPECT_EQ(cl::Sensor::Belly, dataPoints.at(6).sensor());
  EXPECT_EQ(cl::Channel::GyroscopeY, dataPoints.at(6).channel());
  EXPECT_LONG_DOUBLE_EQ(6000.0L, dataPoints.at(6).value());

  EXPECT_EQ("test.csv", dataPoints.at(7).fileName());
  EXPECT_LONG_DOUBLE_EQ(0.01L, dataPoints.at(7).time());
  EXPECT_EQ(cl::Sensor::Belly, dataPoints.at(7).sensor());
  EXPECT_EQ(cl::Channel::GyroscopeZ, dataPoints.at(7).channel());
  EXPECT_LONG_DOUBLE_EQ(7000.0L, dataPoints.at(7).value());
}
