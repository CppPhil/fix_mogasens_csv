#include "gtest/gtest.h"

#include <pl/string_view.hpp>

#include <cl/fs/directory_listing.hpp>

#include <iostream>

namespace {
constexpr char resourcesPath[] = "csv_lib/test/resources";
} // namespace

TEST(directoryListing, shouldFindFiles)
{
  const cl::Expected<std::vector<cl::fs::Path>> result{
    cl::fs::directoryListing(cl::fs::Path{std::string{resourcesPath}})};

  ASSERT_TRUE(result.has_value());

  const std::vector<cl::fs::Path>& listing{*result};

  ASSERT_EQ(2U, listing.size());

  EXPECT_EQ("data_set.csv", listing.front().str());
  EXPECT_EQ("test.csv", listing[1U].str());
}

TEST(directoryListing, shouldFindFilesWithDotAndDotDot)
{
  const cl::Expected<std::vector<cl::fs::Path>> result{cl::fs::directoryListing(
    cl::fs::Path{std::string{resourcesPath}},
    cl::fs::DirectoryListingOption::None)};

  ASSERT_TRUE(result.has_value());

  const std::vector<cl::fs::Path>& listing{*result};

  ASSERT_EQ(4U, listing.size());

  EXPECT_EQ(".", listing.front().str());
  EXPECT_EQ("..", listing[1U].str());
  EXPECT_EQ("data_set.csv", listing[2U].str());
  EXPECT_EQ("test.csv", listing[3U].str());
}

TEST(directoryListing, shouldReturnErrorWhenPathDoesNotExist)
{
  const cl::Expected<std::vector<cl::fs::Path>> result{
    cl::fs::directoryListing(cl::fs::Path{std::string{"herp/derp"}})};

  EXPECT_FALSE(result.has_value());
}
