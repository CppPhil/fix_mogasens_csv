#include <cstring>

#include <sstream>

#include "gtest/gtest.h"

#include <pl/string_view.hpp>

#include "error.hpp"
#include "exception.hpp"

const fe::Error error{
  /* kind */ fe::Error::Filesystem,
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
  EXPECT_EQ(fe::Error::Filesystem, error.kind());
  EXPECT_EQ("test_file.cpp", error.file());
  EXPECT_EQ("bad_function", error.function());
  EXPECT_EQ(48, error.line());
  EXPECT_EQ("Couldn't initialize the flux capacitor.", error.message());
}

TEST(error, shouldThrowExceptionWhenRaiseIsCalled)
{
  EXPECT_THROW(error.raise(), fe::Exception);

  try {
    error.raise();
  }
  catch (const fe::Exception& ex) {
    EXPECT_EQ("test_file.cpp", ex.file());
    EXPECT_EQ("bad_function", ex.function());
    EXPECT_EQ(48, ex.line());
    using namespace std::string_literals;
    EXPECT_EQ("Couldn't initialize the flux capacitor."s, ex.what());
  }
}

namespace {
tl::expected<int, fe::Error> testFunction()
{
  return FE_UNEXPECTED(fe::Error::Filesystem, "Computer says no!");
}
} // namespace

TEST(error, shouldCreateExpectedWithUnexpected)
{
  const tl::expected<int, fe::Error> expected{testFunction()};

  ASSERT_FALSE(expected.has_value());

  const fe::Error& error{expected.error()};

  EXPECT_TRUE(pl::string_view{error.file()}.ends_with("error_test.cpp"));
  EXPECT_NE(nullptr, std::strstr(error.function().c_str(), "testFunction"));
  EXPECT_EQ(57, error.line());
  EXPECT_EQ("Computer says no!", error.message());
}
