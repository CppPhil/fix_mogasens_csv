#include "gtest/gtest.h"

#include <pl/size_t.hpp>
#include <pl/strcontains.hpp>

#include "log_info.hpp"

TEST(logInfo, shouldWork)
{
  using namespace pl::literals::integer_literals;

  constexpr char validPath[]
    = "segmentation_comparison/logs/"
      "skip_window-false_delete_too_close-true_delete_low_variance-false_kind-"
      "max_window-101_filter-butterworth.log";

  const cl::Expected<cs::LogInfo> expected{
    cs::LogInfo::create(cl::fs::Path{std::string{validPath}})};

  ASSERT_TRUE(expected.has_value());

  const cs::LogInfo& logInfo{*expected};

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_FALSE(logInfo.skipWindow());
  EXPECT_TRUE(logInfo.deleteTooClose());
  EXPECT_FALSE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Maxima, logInfo.segmentationKind());
  EXPECT_EQ(101_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());
}

TEST(logInfo, shouldWork2)
{
  using namespace pl::literals::integer_literals;

  constexpr char validPath[]
    = "segmentation_comparison/logs/"
      "skip_window-true_delete_too_close-true_delete_low_variance-false_kind-"
      "max_window-101_filter-butterworth.log";

  const cl::Expected<cs::LogInfo> expected{
    cs::LogInfo::create(cl::fs::Path{std::string{validPath}})};

  ASSERT_TRUE(expected.has_value());

  const cs::LogInfo& logInfo{*expected};

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_TRUE(logInfo.deleteTooClose());
  EXPECT_FALSE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Maxima, logInfo.segmentationKind());
  EXPECT_EQ(101_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());
}

TEST(logInfo, shouldWork3)
{
  using namespace pl::literals::integer_literals;

  constexpr char validPath[]
    = "segmentation_comparison/logs/"
      "skip_window-true_delete_too_close-false_delete_low_variance-false_kind-"
      "max_window-101_filter-butterworth.log";

  const cl::Expected<cs::LogInfo> expected{
    cs::LogInfo::create(cl::fs::Path{std::string{validPath}})};

  ASSERT_TRUE(expected.has_value());

  const cs::LogInfo& logInfo{*expected};

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_FALSE(logInfo.deleteTooClose());
  EXPECT_FALSE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Maxima, logInfo.segmentationKind());
  EXPECT_EQ(101_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());
}

TEST(logInfo, shouldWork4)
{
  using namespace pl::literals::integer_literals;

  constexpr char validPath[]
    = "segmentation_comparison/logs/"
      "skip_window-true_delete_too_close-false_delete_low_variance-true_kind-"
      "max_window-101_filter-butterworth.log";

  const cl::Expected<cs::LogInfo> expected{
    cs::LogInfo::create(cl::fs::Path{std::string{validPath}})};

  ASSERT_TRUE(expected.has_value());

  const cs::LogInfo& logInfo{*expected};

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_FALSE(logInfo.deleteTooClose());
  EXPECT_TRUE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Maxima, logInfo.segmentationKind());
  EXPECT_EQ(101_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());
}

TEST(logInfo, shouldWork5)
{
  using namespace pl::literals::integer_literals;

  constexpr char validPath[]
    = "segmentation_comparison/logs/"
      "skip_window-true_delete_too_close-false_delete_low_variance-true_kind-"
      "min_window-101_filter-butterworth.log";

  const cl::Expected<cs::LogInfo> expected{
    cs::LogInfo::create(cl::fs::Path{std::string{validPath}})};

  ASSERT_TRUE(expected.has_value());

  const cs::LogInfo& logInfo{*expected};

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_FALSE(logInfo.deleteTooClose());
  EXPECT_TRUE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Minima, logInfo.segmentationKind());
  EXPECT_EQ(101_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());
}

TEST(logInfo, shouldWork6)
{
  using namespace pl::literals::integer_literals;

  constexpr char validPath[]
    = "segmentation_comparison/logs/"
      "skip_window-true_delete_too_close-false_delete_low_variance-true_kind-"
      "both_window-101_filter-butterworth.log";

  const cl::Expected<cs::LogInfo> expected{
    cs::LogInfo::create(cl::fs::Path{std::string{validPath}})};

  ASSERT_TRUE(expected.has_value());

  const cs::LogInfo& logInfo{*expected};

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_FALSE(logInfo.deleteTooClose());
  EXPECT_TRUE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Both, logInfo.segmentationKind());
  EXPECT_EQ(101_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());
}

TEST(logInfo, shouldWork7)
{
  using namespace pl::literals::integer_literals;

  constexpr char validPath[]
    = "segmentation_comparison/logs/"
      "skip_window-true_delete_too_close-false_delete_low_variance-true_kind-"
      "both_window-51_filter-butterworth.log";

  const cl::Expected<cs::LogInfo> expected{
    cs::LogInfo::create(cl::fs::Path{std::string{validPath}})};

  ASSERT_TRUE(expected.has_value());

  const cs::LogInfo& logInfo{*expected};

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_FALSE(logInfo.deleteTooClose());
  EXPECT_TRUE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Both, logInfo.segmentationKind());
  EXPECT_EQ(51_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());
}

TEST(logInfo, shouldWork8)
{
  using namespace pl::literals::integer_literals;

  constexpr char validPath[]
    = "segmentation_comparison/logs/"
      "skip_window-true_delete_too_close-false_delete_low_variance-true_kind-"
      "both_window-151_filter-butterworth.log";

  const cl::Expected<cs::LogInfo> expected{
    cs::LogInfo::create(cl::fs::Path{std::string{validPath}})};

  ASSERT_TRUE(expected.has_value());

  const cs::LogInfo& logInfo{*expected};

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_FALSE(logInfo.deleteTooClose());
  EXPECT_TRUE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Both, logInfo.segmentationKind());
  EXPECT_EQ(151_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());
}

TEST(logInfo, shouldWork9)
{
  using namespace pl::literals::integer_literals;

  constexpr char validPath[]
    = "segmentation_comparison/logs/"
      "skip_window-true_delete_too_close-false_delete_low_variance-true_kind-"
      "both_window-151_filter-average.log";

  const cl::Expected<cs::LogInfo> expected{
    cs::LogInfo::create(cl::fs::Path{std::string{validPath}})};

  ASSERT_TRUE(expected.has_value());

  const cs::LogInfo& logInfo{*expected};

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_FALSE(logInfo.deleteTooClose());
  EXPECT_TRUE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Both, logInfo.segmentationKind());
  EXPECT_EQ(151_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::MovingAverage, logInfo.filterKind());
}

TEST(logInfo, shouldResultInErrorIfLogFilePathIsTooShort)
{
  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(
    cl::fs::Path{std::string{"segmentation_comparison/logs/"}})};
  EXPECT_FALSE(expected.has_value());
}

TEST(logInfo, shouldFailIfSkipWindowIsInvalid)
{
  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(cl::fs::Path{
    std::string{"segmentation_comparison/logs/"
                "skip_window-abcd_delete_too_close-true_delete_low_variance-"
                "false_kind-max_window-101_filter-butterworth.log"}})};
  EXPECT_FALSE(expected.has_value());
}

TEST(logInfo, shouldFailIfDeleteTooCloseIsInvalid)
{
  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(cl::fs::Path{
    std::string{"segmentation_comparison/logs/"
                "skip_window-true_delete_too_close-affe_delete_low_variance-"
                "false_kind-max_window-101_filter-butterworth.log"}})};
  EXPECT_FALSE(expected.has_value());
}

TEST(logInfo, shouldFailIfDeleteTooLowVarianceIsInvalid)
{
  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(cl::fs::Path{
    std::string{"segmentation_comparison/logs/"
                "skip_window-true_delete_too_close-true_delete_low_variance-"
                "ZZZZZ_kind-max_window-101_filter-butterworth.log"}})};
  EXPECT_FALSE(expected.has_value());
}

TEST(logInfo, shouldFailIfSegmentationKindIsInvalid)
{
  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(cl::fs::Path{
    std::string{"segmentation_comparison/logs/"
                "skip_window-true_delete_too_close-true_delete_low_variance-"
                "false_kind-mix_window-101_filter-butterworth.log"}})};
  EXPECT_FALSE(expected.has_value());
}

TEST(logInfo, shouldFailIfWindowSizeIsInvalid)
{
  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(cl::fs::Path{
    std::string{"segmentation_comparison/logs/"
                "skip_window-true_delete_too_close-true_delete_low_variance-"
                "false_kind-max_window-GARBAGE_filter-butterworth.log"}})};
  EXPECT_FALSE(expected.has_value());
  EXPECT_TRUE(
    pl::strcontains(expected.error().message(), "did not match the regex"));
}

TEST(logInfo, shouldFailIfFilterIsInvalid)
{
  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(cl::fs::Path{
    std::string{"segmentation_comparison/logs/"
                "skip_window-true_delete_too_close-true_delete_low_variance-"
                "false_kind-min_window-101_filter-abcdefgh.log"}})};
  EXPECT_FALSE(expected.has_value());
}
