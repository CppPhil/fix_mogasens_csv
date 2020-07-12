#include "gtest/gtest.h"

#include "remove_zeros_from_field.hpp"

TEST(removeZerosFromField, shouldRemoveDotAndZeros)
{
  std::string       string{"5454541.0000"};
  const std::string expected{"5454541"};

  fmc::removeZerosFromField(&string);

  EXPECT_EQ(expected, string);
}

TEST(removeZerosFromField, shouldNotRemoveIfNonZerosFollow)
{
  std::string       string{"5642.012340"};
  const std::string expected{string};

  fmc::removeZerosFromField(&string);

  EXPECT_EQ(expected, string);
}

TEST(removeZerosFromField, shouldNotRemoveIfNoDot)
{
  std::string       string{"54152415"};
  const std::string expected{string};

  fmc::removeZerosFromField(&string);

  EXPECT_EQ(expected, string);
}

TEST(removeZerosFromField, shouldDoNothingIfStringIsEmpty)
{
  std::string string{};

  fmc::removeZerosFromField(&string);

  EXPECT_TRUE(string.empty());
}

TEST(removeZerosFromField, shouldDeleteStringIfStringIsSingleDot)
{
  std::string string{"."};

  fmc::removeZerosFromField(&string);

  EXPECT_TRUE(string.empty());
}

TEST(removeZerosFromField, shouldDeleteStringIfStringIsDotAndZero)
{
  std::string string{".0"};

  fmc::removeZerosFromField(&string);

  EXPECT_TRUE(string.empty());
}
