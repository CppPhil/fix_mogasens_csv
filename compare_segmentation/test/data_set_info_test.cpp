#include "gtest/gtest.h"

#include "data_set_info.hpp"

TEST(dataSetInfo, pushUpCount)
{
  EXPECT_EQ(24, cs::pushUpCount("11.17.39"));
  EXPECT_EQ(20, cs::pushUpCount("12.50.00"));
  EXPECT_EQ(15, cs::pushUpCount("13.00.09"));

  EXPECT_EQ(10, cs::pushUpCount("14.59.59"));
  EXPECT_EQ(16, cs::pushUpCount("15.13.22"));
  EXPECT_EQ(18, cs::pushUpCount("15.31.36"));

  EXPECT_EQ(26, cs::pushUpCount("14.07.33"));
  EXPECT_EQ(22, cs::pushUpCount("14.14.32"));
  EXPECT_EQ(18, cs::pushUpCount("14.20.28"));
}
