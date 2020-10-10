#include "gtest/gtest.h"

#include <log_files.hpp>
#include <paths.hpp>

TEST(logFiles, shouldFindLogFiles)
{
  const cl::Expected<std::vector<cl::fs::Path>> expected{
    cs::logFiles(cs::logPath)};

  ASSERT_TRUE(expected.has_value());

  const std::vector<cl::fs::Path>& paths{*expected};

  EXPECT_EQ(528U, paths.size());

  for (const cl::fs::Path& path : paths) {
    const pl::string_view stringView{path.str()};
    EXPECT_TRUE(stringView.ends_with(".log"));
  }
}

TEST(logFiles, shouldFindOldLogFiles)
{
  const cl::Expected<std::vector<cl::fs::Path>> expected{
    cs::logFiles(cs::oldLogPath)};

  ASSERT_TRUE(expected.has_value());

  const std::vector<cl::fs::Path>& paths{*expected};

  EXPECT_EQ(288U, paths.size());

  for (const cl::fs::Path& path : paths) {
    const pl::string_view stringView{path.str()};
    EXPECT_TRUE(stringView.ends_with(".log"));
  }
}

TEST(logFiles, shouldNotFindGarbage)
{
  const cl::Expected<std::vector<cl::fs::Path>> expected{
    cs::logFiles("not/a/real/path")};

  EXPECT_FALSE(expected.has_value());
}
