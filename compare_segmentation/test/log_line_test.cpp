#include <string>

#include "gtest/gtest.h"

#include "log_line.hpp"

using namespace std::string_literals;

TEST(LogLine, shouldWorkWithPreprocessedLine)
{
  const cl::Expected<cs::LogLine> expectedLogLine{cs::LogLine::parse(
    R"(preprocessed_segment.py: 62 segmentation points found in "resources/preprocessed/2020-07-02_14.07.33_Belly.csv".)")};

  ASSERT_TRUE(expectedLogLine.has_value());

  const cs::LogLine& logLine{*expectedLogLine};

  EXPECT_EQ(UINT64_C(62), logLine.segmentationPointCount());
  EXPECT_EQ(
    "resources/preprocessed/2020-07-02_14.07.33_Belly.csv"s,
    logLine.filePath());
  EXPECT_EQ("14.07.33"s, logLine.shortFileName());
  EXPECT_EQ(UINT64_C(770), logLine.sensor());
}

TEST(LogLine, shouldWorkWithOldLine)
{
  const cl::Expected<cs::LogLine> expectedLogLine{cs::LogLine::parse(
    R"(segment.py: 29 segmentation points found in "resources/Marcelle/2020-07-02_15.31.36_out_avg_filter_125.csv".)")};

  ASSERT_TRUE(expectedLogLine.has_value());

  const cs::LogLine& logLine{*expectedLogLine};

  EXPECT_EQ(UINT64_C(29), logLine.segmentationPointCount());
  EXPECT_EQ(
    "resources/Marcelle/2020-07-02_15.31.36_out_avg_filter_125.csv"s,
    logLine.filePath());
  EXPECT_EQ("15.31.36"s, logLine.shortFileName());
  EXPECT_EQ(cs::LogLine::invalidSensor, logLine.sensor());
}

TEST(LogLine, shouldNotMatchGarbage)
{
  const cl::Expected<cs::LogLine> expected{
    cs::LogLine::parse("ntoeuhnstoehinoeuhiteunsi0")};
  EXPECT_FALSE(expected.has_value());
}

TEST(LogLine, shouldNotWorkIfThereIsNoUnderscore)
{
  const cl::Expected<cs::LogLine> expectedLogLine{cs::LogLine::parse(
    R"(segment.py: 29 segmentation points found in "resources/Marcelle/2020-07-02-15.31.36-out-avg-filter-125.csv".)")};
  EXPECT_FALSE(expectedLogLine.has_value());
}

TEST(LogLine, shouldNotParseGarbageSensor)
{
  const cl::Expected<cs::LogLine> expectedLogLine{cs::LogLine::parse(
    R"(preprocessed_segment.py: 62 segmentation points found in "resources/preprocessed/2020-07-02_14.07.33_GarbageSensor.csv".)")};
  EXPECT_FALSE(expectedLogLine.has_value());
}
