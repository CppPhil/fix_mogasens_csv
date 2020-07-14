#include <type_traits>

#include "gtest/gtest.h"

#include "column.hpp"

TEST(column, shouldHaveCorrectIndex)
{
  EXPECT_EQ(0, cl::column_index<cl::Column::Time>);
  EXPECT_EQ(1, cl::column_index<cl::Column::HardwareTimestamp>);
  EXPECT_EQ(2, cl::column_index<cl::Column::ExtractId>);
  EXPECT_EQ(3, cl::column_index<cl::Column::Trigger>);
  EXPECT_EQ(4, cl::column_index<cl::Column::AccelerometerX>);
  EXPECT_EQ(5, cl::column_index<cl::Column::AccelerometerY>);
  EXPECT_EQ(6, cl::column_index<cl::Column::AccelerometerZ>);
  EXPECT_EQ(7, cl::column_index<cl::Column::GyroscopeX>);
  EXPECT_EQ(8, cl::column_index<cl::Column::GyroscopeY>);
  EXPECT_EQ(9, cl::column_index<cl::Column::GyroscopeZ>);
  EXPECT_EQ(10, cl::column_index<cl::Column::SamplingRate>);
}

TEST(column, shouldHaveCorrectColumnType)
{
  EXPECT_TRUE((std::is_same_v<cl::column_type<cl::Column::Time>, long double>));
  EXPECT_TRUE((std::is_same_v<
               cl::column_type<cl::Column::HardwareTimestamp>,
               std::uint64_t>));
  EXPECT_TRUE(
    (std::is_same_v<cl::column_type<cl::Column::ExtractId>, cl::Sensor>));
  EXPECT_TRUE(
    (std::is_same_v<cl::column_type<cl::Column::Trigger>, std::uint64_t>));
  EXPECT_TRUE(
    (std::is_same_v<cl::column_type<cl::Column::AccelerometerX>, long double>));
  EXPECT_TRUE(
    (std::is_same_v<cl::column_type<cl::Column::AccelerometerY>, long double>));
  EXPECT_TRUE(
    (std::is_same_v<cl::column_type<cl::Column::AccelerometerZ>, long double>));
  EXPECT_TRUE(
    (std::is_same_v<cl::column_type<cl::Column::GyroscopeX>, long double>));
  EXPECT_TRUE(
    (std::is_same_v<cl::column_type<cl::Column::GyroscopeY>, long double>));
  EXPECT_TRUE(
    (std::is_same_v<cl::column_type<cl::Column::GyroscopeZ>, long double>));
  EXPECT_TRUE(
    (std::is_same_v<cl::column_type<cl::Column::SamplingRate>, std::uint64_t>));
}
