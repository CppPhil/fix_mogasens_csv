#include "gtest/gtest.h"

#include "percentage_of.hpp"

#ifndef EXPECT_LONG_DOUBLE_EQ
#define EXPECT_LONG_DOUBLE_EQ(a, b) \
  EXPECT_DOUBLE_EQ(static_cast<double>(a), static_cast<double>(b))
#endif

TEST(percentageOf, shouldWork)
{
  EXPECT_LONG_DOUBLE_EQ(40.0L, ctg::percentageOf(2.0L, 5.0L));
}
