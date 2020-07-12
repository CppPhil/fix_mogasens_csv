#include "gtest/gtest.h"

#include <pl/algo/ranged_algorithms.hpp>
#include <pl/string_view.hpp>

#include "read_csv_file.hpp"

TEST(readCsvFile, shouldReadCsvFile)
{
  constexpr char csvFilePath[] = "feature_extraction/test/resources/test.csv";
  const fe::Expected<std::vector<std::vector<std::string>>> result{
    fe::readCsvFile(csvFilePath)};

  ASSERT_TRUE(result.has_value());

  const std::vector<std::vector<std::string>>& data{*result};
  const std::vector<std::vector<std::string>>  expected{
    {"1", "2", "3", "1", "1", "1", "1", "1", "1", "1"},
    {"4", "5", "6", "2", "2", "2", "2", "2", "2", "2"}};
  EXPECT_EQ(expected, data);
}

namespace {
bool strContains(pl::string_view haystack, pl::string_view needle)
{
  return pl::algo::search(haystack, needle) != haystack.end();
}
} // namespace

TEST(readCsvFile, shouldNotReadNonexistantCsvFile)
{
  const fe::Expected<std::vector<std::vector<std::string>>> result{
    fe::readCsvFile("feature_extraction/test/resources/garbage.csv")};

  ASSERT_FALSE(result.has_value());

  const fe::Error& error{result.error()};

  EXPECT_EQ(fe::Error::Filesystem, error.kind());
  EXPECT_TRUE(pl::string_view{error.file()}.ends_with("read_csv_file.cpp"));
  EXPECT_TRUE(strContains(error.function(), "readCsvFile"));
  EXPECT_EQ(71, error.line());
  EXPECT_EQ(
    "Cannot open file feature_extraction/test/resources/garbage.csv",
    error.message());
}
