#include <cstring>

#include <sstream>

#include "gtest/gtest.h"

#include <pl/string_view.hpp>

#include "error.hpp"
#include "exception.hpp"

const cl::Error error{
  /* kind */ cl::Error::Filesystem,
  /* file */ "test_file.cpp",
  /* function */ "bad_function",
  /* line */ 48,
  /* message */ "Couldn't initialize the flux capacitor."};

TEST(error, shouldPrint)
{
  std::ostringstream oss{};
  oss << error;
  const std::string expected{
    R"(Error{"kind": "Filesystem", "file": "test_file.cpp", "function": "bad_function", "line": 48, "message": "Couldn't initialize the flux capacitor."})"};
  const std::string actual{oss.str()};
  EXPECT_EQ(expected, actual);
}

TEST(error, shouldReturnValues)
{
  EXPECT_EQ(cl::Error::Filesystem, error.kind());
  EXPECT_EQ("test_file.cpp", error.file());
  EXPECT_EQ("bad_function", error.function());
  EXPECT_EQ("Couldn't initialize the flux capacitor.", error.message());
}

TEST(error, shouldThrowExceptionWhenRaiseIsCalled)
{
  EXPECT_THROW(error.raise(), cl::Exception);

  try {
    error.raise();
  }
  catch (const cl::Exception& ex) {
    EXPECT_EQ("test_file.cpp", ex.file());
    EXPECT_EQ("bad_function", ex.function());
    using namespace std::string_literals;
    EXPECT_EQ("Couldn't initialize the flux capacitor."s, ex.what());
  }
}

namespace {
tl::expected<int, cl::Error> testFunction()
{
  return CL_UNEXPECTED(cl::Error::Filesystem, "Computer says no!");
}
} // namespace

TEST(error, shouldCreateExpectedWithUnexpected)
{
  const tl::expected<int, cl::Error> expected{testFunction()};

  ASSERT_FALSE(expected.has_value());

  const cl::Error& error{expected.error()};

  EXPECT_TRUE(pl::string_view{error.file()}.ends_with("error_test.cpp"));
  EXPECT_NE(nullptr, std::strstr(error.function().c_str(), "testFunction"));
  EXPECT_EQ("Computer says no!", error.message());
}
