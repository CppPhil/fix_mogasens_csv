#include "gtest/gtest.h"

#include "data_set_info.hpp"

TEST(dataSetInfo, repetitionCount)
{
  EXPECT_EQ(24, cs::repetitionCount("11.17.39"));
  EXPECT_EQ(20, cs::repetitionCount("12.50.00"));
  EXPECT_EQ(15, cs::repetitionCount("13.00.09"));

  EXPECT_EQ(10, cs::repetitionCount("14.59.59"));
  EXPECT_EQ(16, cs::repetitionCount("15.13.22"));
  EXPECT_EQ(18, cs::repetitionCount("15.31.36"));

  EXPECT_EQ(26, cs::repetitionCount("14.07.33"));
  EXPECT_EQ(22, cs::repetitionCount("14.14.32"));
  EXPECT_EQ(18, cs::repetitionCount("14.20.28"));
}
