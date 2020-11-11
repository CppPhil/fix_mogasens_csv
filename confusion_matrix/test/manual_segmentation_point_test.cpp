#include <cstddef>

#include <array>

#include "gtest/gtest.h"

#include "manual_segmentation_point.hpp"

TEST(ManualSegmentationPoint, shouldConstruct)
{
  const cm::ManualSegmentationPoint segmentationPoint{1, 2, 3, 4};

  EXPECT_EQ(1, segmentationPoint.hour());
  EXPECT_EQ(2, segmentationPoint.minute());
  EXPECT_EQ(3, segmentationPoint.second());
  EXPECT_EQ(4, segmentationPoint.frame());
}

TEST(ManualSegmentationPoint, shouldThrowWhenConstructingWithInvalidMinute)
{
  ASSERT_THROW((cm::ManualSegmentationPoint{1, 60, 3, 4}), cl::Exception);

  try {
    cm::ManualSegmentationPoint{1, 60, 3, 4};
  }
  catch (const cl::Exception& ex) {
    EXPECT_STREQ("Invalid minute: 60 is not between 0 and 59!", ex.what());
  }
}

TEST(ManualSegmentationPoint, shouldThrowWhenConstructingWithInvalidSecond)
{
  ASSERT_THROW((cm::ManualSegmentationPoint{1, 2, 60, 4}), cl::Exception);

  try {
    cm::ManualSegmentationPoint{1, 2, 60, 4};
  }
  catch (const cl::Exception& ex) {
    EXPECT_STREQ("Invalid second: 60 is not between 0 and 59!", ex.what());
  }
}

TEST(ManualSegmentationPoint, shouldThrowWhenConstructingWithInvalidFrame)
{
  ASSERT_THROW((cm::ManualSegmentationPoint{1, 2, 3, 30}), cl::Exception);

  try {
    cm::ManualSegmentationPoint{1, 2, 3, 30};
  }
  catch (const cl::Exception& ex) {
    EXPECT_STREQ("Invalid frame: 30 is not between 0 and 29!", ex.what());
  }
}

TEST(ManualSegmentationPoint, shouldConvertToMilliseconds)
{
  const cm::ManualSegmentationPoint point{5, 59, 59, 29};
  EXPECT_EQ(21599967, point.asMilliseconds());
}

TEST(ManualSegmentationPoint, shouldConvertHourToMilliseconds)
{
  const cm::ManualSegmentationPoint point{1, 0, 0, 0};
  EXPECT_EQ(3600000, point.asMilliseconds());
}

TEST(ManualSegmentationPoint, shouldConvertMinuteToMilliseconds)
{
  const cm::ManualSegmentationPoint point{0, 1, 0, 0};
  EXPECT_EQ(60000, point.asMilliseconds());
}

TEST(ManualSegmentationPoint, shouldConvertSecondToMilliseconds)
{
  const cm::ManualSegmentationPoint point{0, 0, 1, 0};
  EXPECT_EQ(1000, point.asMilliseconds());
}

TEST(ManualSegmentationPoint, shouldConvertFramesToMilliseconds)
{
  constexpr std::size_t framesPerSecond{30};

  constexpr std::array<std::uint64_t, framesPerSecond> milliseconds{
    {0,   33,  67,  100, 133, 167, 200, 233, 267, 300,
     333, 367, 400, 433, 467, 500, 533, 567, 600, 633,
     667, 700, 733, 767, 800, 833, 867, 900, 933, 967}};

  for (std::size_t frame{0}; frame < framesPerSecond; ++frame) {
    const cm::ManualSegmentationPoint point{
      0, 0, 0, static_cast<std::uint32_t>(frame)};
    EXPECT_EQ(milliseconds[frame], point.asMilliseconds());
  }
}
