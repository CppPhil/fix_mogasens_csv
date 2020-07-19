#include "gtest/gtest.h"

#include <pl/string_view.hpp>

#include "cl/s2n.hpp"

TEST(s2n, shouldWork)
{
  EXPECT_EQ(5, cl::s2n<int>("5").value());
  EXPECT_EQ(213L, cl::s2n<long>("213").value());
  EXPECT_EQ(32133254LL, cl::s2n<long long>("32133254").value());
  EXPECT_EQ(654625UL, cl::s2n<unsigned long>("654625").value());
  EXPECT_EQ(23141234458ULL, cl::s2n<unsigned long long>("23141234458").value());
  EXPECT_FLOAT_EQ(123.0F, cl::s2n<float>("123.0").value());
  EXPECT_DOUBLE_EQ(1254.542, cl::s2n<double>("1254.542").value());
  EXPECT_DOUBLE_EQ(
    54251.454212,
    static_cast<double>(cl::s2n<long double>("54251.454212").value()));
}

TEST(s2n, shouldReturnInvalidArgumentErrorIfInputIsInvalid)
{
  const auto exp{cl::s2n<long>("noethunaots")};
  ASSERT_FALSE(exp.has_value());
  EXPECT_TRUE(pl::string_view{exp.error().message()}.starts_with(
    "s2n: caught invalid_argument: \""));
}

TEST(s2n, shouldReturnOutOfRangeErrorIfInputIsOutOfRange)
{
  const auto exp{cl::s2n<int>("FFFFFFFFFFFFFFFF", nullptr, 16)};
  ASSERT_FALSE(exp.has_value());
  EXPECT_TRUE(pl::string_view{exp.error().message()}.starts_with(
    "s2n: caught out_of_range: \""));
}
