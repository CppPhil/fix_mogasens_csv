#include "gtest/gtest.h"

#include "split_string.hpp"

TEST(splitString, shouldSplitString)
{
  EXPECT_EQ(
    (std::vector<std::string>{"test", "text", "test2"}),
    cm::splitString("test;text;test2", ";"));
}
