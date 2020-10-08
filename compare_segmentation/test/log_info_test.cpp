#include "gtest/gtest.h"

#include <pl/size_t.hpp>
#include <pl/strcontains.hpp>

#include "log_info.hpp"

TEST(LogInfo, shouldWork)
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

  ASSERT_TRUE(logInfo.isInitialized());

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_FALSE(logInfo.skipWindow());
  EXPECT_TRUE(logInfo.deleteTooClose());
  EXPECT_FALSE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Maxima, logInfo.segmentationKind());
  EXPECT_EQ(101_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());

  EXPECT_EQ(cs::LogInfo::invalidSensor, logInfo.sensor());
}

TEST(LogInfo, shouldWork2)
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

  ASSERT_TRUE(logInfo.isInitialized());

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_TRUE(logInfo.deleteTooClose());
  EXPECT_FALSE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Maxima, logInfo.segmentationKind());
  EXPECT_EQ(101_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());

  EXPECT_EQ(cs::LogInfo::invalidSensor, logInfo.sensor());
}

TEST(LogInfo, shouldWork3)
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

  ASSERT_TRUE(logInfo.isInitialized());

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_FALSE(logInfo.deleteTooClose());
  EXPECT_FALSE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Maxima, logInfo.segmentationKind());
  EXPECT_EQ(101_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());

  EXPECT_EQ(cs::LogInfo::invalidSensor, logInfo.sensor());
}

TEST(LogInfo, shouldWork4)
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

  ASSERT_TRUE(logInfo.isInitialized());

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_FALSE(logInfo.deleteTooClose());
  EXPECT_TRUE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Maxima, logInfo.segmentationKind());
  EXPECT_EQ(101_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());

  EXPECT_EQ(cs::LogInfo::invalidSensor, logInfo.sensor());
}

TEST(LogInfo, shouldWork5)
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

  ASSERT_TRUE(logInfo.isInitialized());

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_FALSE(logInfo.deleteTooClose());
  EXPECT_TRUE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Minima, logInfo.segmentationKind());
  EXPECT_EQ(101_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());

  EXPECT_EQ(cs::LogInfo::invalidSensor, logInfo.sensor());
}

TEST(LogInfo, shouldWork6)
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

  ASSERT_TRUE(logInfo.isInitialized());

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_FALSE(logInfo.deleteTooClose());
  EXPECT_TRUE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Both, logInfo.segmentationKind());
  EXPECT_EQ(101_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());

  EXPECT_EQ(cs::LogInfo::invalidSensor, logInfo.sensor());
}

TEST(LogInfo, shouldWork7)
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

  ASSERT_TRUE(logInfo.isInitialized());

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_FALSE(logInfo.deleteTooClose());
  EXPECT_TRUE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Both, logInfo.segmentationKind());
  EXPECT_EQ(51_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());

  EXPECT_EQ(cs::LogInfo::invalidSensor, logInfo.sensor());
}

TEST(LogInfo, shouldWork8)
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

  ASSERT_TRUE(logInfo.isInitialized());

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_FALSE(logInfo.deleteTooClose());
  EXPECT_TRUE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Both, logInfo.segmentationKind());
  EXPECT_EQ(151_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::Butterworth, logInfo.filterKind());

  EXPECT_EQ(cs::LogInfo::invalidSensor, logInfo.sensor());
}

TEST(LogInfo, shouldWork9)
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

  ASSERT_TRUE(logInfo.isInitialized());

  EXPECT_EQ(std::string{validPath}, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_FALSE(logInfo.deleteTooClose());
  EXPECT_TRUE(logInfo.deleteLowVariance());
  EXPECT_EQ(cs::SegmentationKind::Both, logInfo.segmentationKind());
  EXPECT_EQ(151_zu, logInfo.windowSize());
  EXPECT_EQ(cs::FilterKind::MovingAverage, logInfo.filterKind());

  EXPECT_EQ(cs::LogInfo::invalidSensor, logInfo.sensor());
}

TEST(LogInfo, shouldWorkWithOldPath)
{
  using namespace pl::literals::integer_literals;

  const std::string path{
    "segmentation_comparison/logs/old/"
    "skip_window-true_delete_too_close-true_delete_low_variance-true_sensor-"
    "770_kind-both_window-151.log"};

  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(path)};

  ASSERT_TRUE(expected.has_value());

  const cs::LogInfo& logInfo{*expected};

  ASSERT_TRUE(logInfo.isInitialized());

  EXPECT_EQ(path, logInfo.logFilePath());
  EXPECT_TRUE(logInfo.skipWindow());
  EXPECT_TRUE(logInfo.deleteTooClose());
  EXPECT_TRUE(logInfo.deleteLowVariance());
  EXPECT_EQ(UINT64_C(770), logInfo.sensor());
  EXPECT_EQ(cs::SegmentationKind::Both, logInfo.segmentationKind());
  EXPECT_EQ(151_zu, logInfo.windowSize());

  EXPECT_EQ(cs::FilterKind::MovingAverage, logInfo.filterKind());
}

TEST(LogInfo, shouldWorkWithOldPath2)
{
  using namespace pl::literals::integer_literals;

  const std::string path{
    "segmentation_comparison/logs/old/"
    "skip_window-false_delete_too_close-false_delete_low_variance-false_sensor-"
    "769_kind-both_window-101.log"};

  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(path)};

  ASSERT_TRUE(expected.has_value());

  const cs::LogInfo& logInfo{*expected};

  ASSERT_TRUE(logInfo.isInitialized());

  EXPECT_EQ(path, logInfo.logFilePath());
  EXPECT_FALSE(logInfo.skipWindow());
  EXPECT_FALSE(logInfo.deleteTooClose());
  EXPECT_FALSE(logInfo.deleteLowVariance());
  EXPECT_EQ(UINT64_C(769), logInfo.sensor());
  EXPECT_EQ(cs::SegmentationKind::Both, logInfo.segmentationKind());
  EXPECT_EQ(101_zu, logInfo.windowSize());

  EXPECT_EQ(cs::FilterKind::MovingAverage, logInfo.filterKind());
}

TEST(LogInfo, shouldResultInErrorIfLogFilePathIsTooShort)
{
  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(
    cl::fs::Path{std::string{"segmentation_comparison/logs/"}})};
  EXPECT_FALSE(expected.has_value());
}

TEST(LogInfo, shouldFailIfSkipWindowIsInvalid)
{
  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(cl::fs::Path{
    std::string{"segmentation_comparison/logs/"
                "skip_window-abcd_delete_too_close-true_delete_low_variance-"
                "false_kind-max_window-101_filter-butterworth.log"}})};
  EXPECT_FALSE(expected.has_value());
}

TEST(LogInfo, shouldFailIfDeleteTooCloseIsInvalid)
{
  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(cl::fs::Path{
    std::string{"segmentation_comparison/logs/"
                "skip_window-true_delete_too_close-affe_delete_low_variance-"
                "false_kind-max_window-101_filter-butterworth.log"}})};
  EXPECT_FALSE(expected.has_value());
}

TEST(LogInfo, shouldFailIfDeleteTooLowVarianceIsInvalid)
{
  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(cl::fs::Path{
    std::string{"segmentation_comparison/logs/"
                "skip_window-true_delete_too_close-true_delete_low_variance-"
                "ZZZZZ_kind-max_window-101_filter-butterworth.log"}})};
  EXPECT_FALSE(expected.has_value());
}

TEST(LogInfo, shouldFailIfSegmentationKindIsInvalid)
{
  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(cl::fs::Path{
    std::string{"segmentation_comparison/logs/"
                "skip_window-true_delete_too_close-true_delete_low_variance-"
                "false_kind-mix_window-101_filter-butterworth.log"}})};
  EXPECT_FALSE(expected.has_value());
}

TEST(LogInfo, shouldFailIfWindowSizeIsInvalid)
{
  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(cl::fs::Path{
    std::string{"segmentation_comparison/logs/"
                "skip_window-true_delete_too_close-true_delete_low_variance-"
                "false_kind-max_window-GARBAGE_filter-butterworth.log"}})};
  EXPECT_FALSE(expected.has_value());
  EXPECT_TRUE(
    pl::strcontains(expected.error().message(), "did not match the regex"));
}

TEST(LogInfo, shouldFailIfFilterIsInvalid)
{
  const cl::Expected<cs::LogInfo> expected{cs::LogInfo::create(cl::fs::Path{
    std::string{"segmentation_comparison/logs/"
                "skip_window-true_delete_too_close-true_delete_low_variance-"
                "false_kind-min_window-101_filter-abcdefgh.log"}})};
  EXPECT_FALSE(expected.has_value());
}

TEST(LogInfo, shouldCreateUninitialzedObjectWhenDefaultConstructorIsCalled)
{
  const cs::LogInfo logInfo{};
  EXPECT_FALSE(logInfo.isInitialized());
}
