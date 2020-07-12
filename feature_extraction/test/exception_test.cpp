#include "gtest/gtest.h"

#include "exception.hpp"

TEST(exception, shouldWork)
{
  const fe::Exception ex1{
    "file.cpp", "function", 25, std::string{"error message"}};
  const fe::Exception ex2{"file.cpp", "function", 25, "error message"};

  EXPECT_EQ("file.cpp", ex1.file());
  EXPECT_EQ("file.cpp", ex2.file());

  EXPECT_EQ("function", ex1.function());
  EXPECT_EQ("function", ex2.function());

  using namespace std::string_literals;
  EXPECT_EQ("error message"s, ex1.what());
  EXPECT_EQ("error message"s, ex2.what());
}
