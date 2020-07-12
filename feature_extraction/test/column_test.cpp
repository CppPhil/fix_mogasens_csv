#include <type_traits>

#include "gtest/gtest.h"

#include "column.hpp"

TEST(column, shouldHaveCorrectIndex)
{
  EXPECT_EQ(0, fe::column_index<fe::Column::Time>);
  EXPECT_EQ(1, fe::column_index<fe::Column::HardwareTimestamp>);
  EXPECT_EQ(2, fe::column_index<fe::Column::ExtractId>);
  EXPECT_EQ(3, fe::column_index<fe::Column::Trigger>);
  EXPECT_EQ(4, fe::column_index<fe::Column::AccelerometerX>);
  EXPECT_EQ(5, fe::column_index<fe::Column::AccelerometerY>);
  EXPECT_EQ(6, fe::column_index<fe::Column::AccelerometerZ>);
  EXPECT_EQ(7, fe::column_index<fe::Column::GyroscopeX>);
  EXPECT_EQ(8, fe::column_index<fe::Column::GyroscopeY>);
  EXPECT_EQ(9, fe::column_index<fe::Column::GyroscopeZ>);
  EXPECT_EQ(10, fe::column_index<fe::Column::SamplingRate>);
}

TEST(column, shouldHaveCorrectColumnType)
{
  EXPECT_TRUE((std::is_same_v<fe::column_type<fe::Column::Time>, long double>));
  EXPECT_TRUE((std::is_same_v<
               fe::column_type<fe::Column::HardwareTimestamp>,
               std::uint64_t>));
  EXPECT_TRUE(
    (std::is_same_v<fe::column_type<fe::Column::ExtractId>, std::uint64_t>));
  EXPECT_TRUE(
    (std::is_same_v<fe::column_type<fe::Column::Trigger>, std::uint64_t>));
  EXPECT_TRUE(
    (std::is_same_v<fe::column_type<fe::Column::AccelerometerX>, long double>));
  EXPECT_TRUE(
    (std::is_same_v<fe::column_type<fe::Column::AccelerometerY>, long double>));
  EXPECT_TRUE(
    (std::is_same_v<fe::column_type<fe::Column::AccelerometerZ>, long double>));
  EXPECT_TRUE(
    (std::is_same_v<fe::column_type<fe::Column::GyroscopeX>, long double>));
  EXPECT_TRUE(
    (std::is_same_v<fe::column_type<fe::Column::GyroscopeY>, long double>));
  EXPECT_TRUE(
    (std::is_same_v<fe::column_type<fe::Column::GyroscopeZ>, long double>));
  EXPECT_TRUE(
    (std::is_same_v<fe::column_type<fe::Column::SamplingRate>, std::uint64_t>));
}
