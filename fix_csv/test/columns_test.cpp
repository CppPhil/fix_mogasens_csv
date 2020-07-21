#include "gtest/gtest.h"

#include "columns.hpp"

TEST(columns, shouldHaveCorrect1BasedIndices)
{
  EXPECT_EQ(1, fmc::timeColumn);
  EXPECT_EQ(2, fmc::hardwareTimestampColumn);
  EXPECT_EQ(3, fmc::extractIdColumn);
  EXPECT_EQ(4, fmc::triggerColumn);
  EXPECT_EQ(5, fmc::accelerometerXColumn);
  EXPECT_EQ(6, fmc::accelerometerYColumn);
  EXPECT_EQ(7, fmc::accelerometerZColumn);
  EXPECT_EQ(8, fmc::gyroscopeXColumn);
  EXPECT_EQ(9, fmc::gyroscopeYColumn);
  EXPECT_EQ(10, fmc::gyroscopeZColumn);
  EXPECT_EQ(11, fmc::samplingRateColumn);
}
