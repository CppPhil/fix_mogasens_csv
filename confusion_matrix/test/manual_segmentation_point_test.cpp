#include <cstddef>

#include <array>
#include <ostream>
#include <sstream>
#include <string>

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

TEST(ManualSegmentationPoint, shouldBeAbleToImportCsvFile)
{
  const std::unordered_map<
    cm::DataSetIdentifier,
    std::vector<cm::ManualSegmentationPoint>>
    map{cm::ManualSegmentationPoint::readCsvFile()};

  const auto segmentationPoints = [&map](cm::DataSetIdentifier dsi)
    -> const std::vector<cm::ManualSegmentationPoint>& {
    const auto it{map.find(dsi)};

    if (it == map.end()) {
      throw std::runtime_error{
        "Couldn't find DataSetIdentifier enumerator in map!"};
    }

    const std::vector<cm::ManualSegmentationPoint>& segmentationPoints{
      it->second};
    return segmentationPoints;
  };

  const auto size = [&segmentationPoints](cm::DataSetIdentifier dsi) {
    return segmentationPoints(dsi).size();
  };

  // Check sizes.
  constexpr std::size_t felix_11_17_39_expectedSize{49};
  constexpr std::size_t felix_12_50_00_expectedSize{41};
  constexpr std::size_t felix_13_00_09_expectedSize{29};
  constexpr std::size_t mike_14_07_33_expectedSize{51};
  constexpr std::size_t mike_14_14_32_expectedSize{44};
  constexpr std::size_t mike_14_20_28_expectedSize{36};
  constexpr std::size_t marsi_14_59_59_expectedSize{20};
  constexpr std::size_t marsi_15_13_22_expectedSize{32};
  constexpr std::size_t marsi_15_31_36_expectedSize{35};
  constexpr std::size_t jan_1_expectedSize{50};
  constexpr std::size_t jan_2_expectedSize{38};
  constexpr std::size_t jan_3_expectedSize{26};
  constexpr std::size_t andre_1_expectedSize{54};
  constexpr std::size_t andre_2_expectedSize{40};
  constexpr std::size_t andre_3_expectedSize{35};
  constexpr std::size_t andre_squats_1_expectedSize{58};
  constexpr std::size_t andre_squats_2_expectedSize{98};
  constexpr std::size_t lucas_1_expectedSize{48};
  constexpr std::size_t lucas_2_expectedSize{38};
  constexpr std::size_t lucas_3_expectedSize{22};

#define DSI ::cm::DataSetIdentifier
  EXPECT_EQ(felix_11_17_39_expectedSize, size(DSI::Felix_11_17_39));
  EXPECT_EQ(felix_12_50_00_expectedSize, size(DSI::Felix_12_50_00));
  EXPECT_EQ(felix_13_00_09_expectedSize, size(DSI::Felix_13_00_09));
  EXPECT_EQ(mike_14_07_33_expectedSize, size(DSI::Mike_14_07_33));
  EXPECT_EQ(mike_14_14_32_expectedSize, size(DSI::Mike_14_14_32));
  EXPECT_EQ(mike_14_20_28_expectedSize, size(DSI::Mike_14_20_28));
  EXPECT_EQ(marsi_14_59_59_expectedSize, size(DSI::Marsi_14_59_59));
  EXPECT_EQ(marsi_15_13_22_expectedSize, size(DSI::Marsi_15_13_22));
  EXPECT_EQ(marsi_15_31_36_expectedSize, size(DSI::Marsi_15_31_36));
  EXPECT_EQ(jan_1_expectedSize, size(DSI::Jan_1));
  EXPECT_EQ(jan_2_expectedSize, size(DSI::Jan_2));
  EXPECT_EQ(jan_3_expectedSize, size(DSI::Jan_3));
  EXPECT_EQ(andre_1_expectedSize, size(DSI::Andre_1));
  EXPECT_EQ(andre_2_expectedSize, size(DSI::Andre_2));
  EXPECT_EQ(andre_3_expectedSize, size(DSI::Andre_3));
  EXPECT_EQ(andre_squats_1_expectedSize, size(DSI::Andre_Squats_1));
  EXPECT_EQ(andre_squats_2_expectedSize, size(DSI::Andre_Squats_2));
  EXPECT_EQ(lucas_1_expectedSize, size(DSI::Lucas_1));
  EXPECT_EQ(lucas_2_expectedSize, size(DSI::Lucas_2));
  EXPECT_EQ(lucas_3_expectedSize, size(DSI::Lucas_3));

  // Check values.
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 9, 20}),
    segmentationPoints(DSI::Felix_11_17_39).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 10, 15}),
    segmentationPoints(DSI::Felix_11_17_39).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 11, 12}),
    segmentationPoints(DSI::Felix_11_17_39).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 16, 8}),
    segmentationPoints(DSI::Felix_12_50_00).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 17, 12}),
    segmentationPoints(DSI::Felix_12_50_00).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 18, 8}),
    segmentationPoints(DSI::Felix_12_50_00).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 18, 8}),
    segmentationPoints(DSI::Felix_13_00_09).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 19, 17}),
    segmentationPoints(DSI::Felix_13_00_09).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 20, 13}),
    segmentationPoints(DSI::Felix_13_00_09).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 10, 28}),
    segmentationPoints(DSI::Mike_14_07_33).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 11, 16}),
    segmentationPoints(DSI::Mike_14_07_33).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 12, 5}),
    segmentationPoints(DSI::Mike_14_07_33).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 10, 26}),
    segmentationPoints(DSI::Mike_14_14_32).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 11, 14}),
    segmentationPoints(DSI::Mike_14_14_32).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 12, 0}),
    segmentationPoints(DSI::Mike_14_14_32).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 12, 18}),
    segmentationPoints(DSI::Mike_14_20_28).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 13, 4}),
    segmentationPoints(DSI::Mike_14_20_28).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 13, 24}),
    segmentationPoints(DSI::Mike_14_20_28).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 12, 4}),
    segmentationPoints(DSI::Marsi_14_59_59).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 13, 1}),
    segmentationPoints(DSI::Marsi_14_59_59).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 14, 9}),
    segmentationPoints(DSI::Marsi_14_59_59).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 10, 29}),
    segmentationPoints(DSI::Marsi_15_13_22).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 11, 25}),
    segmentationPoints(DSI::Marsi_15_13_22).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 12, 23}),
    segmentationPoints(DSI::Marsi_15_13_22).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 13, 18}),
    segmentationPoints(DSI::Marsi_15_31_36).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 14, 12}),
    segmentationPoints(DSI::Marsi_15_31_36).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 15, 17}),
    segmentationPoints(DSI::Marsi_15_31_36).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 4, 27}),
    segmentationPoints(DSI::Jan_1).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 5, 10}),
    segmentationPoints(DSI::Jan_1).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 6, 4}),
    segmentationPoints(DSI::Jan_1).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 4, 29}),
    segmentationPoints(DSI::Jan_2).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 5, 17}),
    segmentationPoints(DSI::Jan_2).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 6, 11}),
    segmentationPoints(DSI::Jan_2).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 4, 10}),
    segmentationPoints(DSI::Jan_3).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 4, 27}),
    segmentationPoints(DSI::Jan_3).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 5, 22}),
    segmentationPoints(DSI::Jan_3).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 2, 27}),
    segmentationPoints(DSI::Andre_1).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 3, 22}),
    segmentationPoints(DSI::Andre_1).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 4, 24}),
    segmentationPoints(DSI::Andre_1).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 2, 7}),
    segmentationPoints(DSI::Andre_2).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 3, 8}),
    segmentationPoints(DSI::Andre_2).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 4, 6}),
    segmentationPoints(DSI::Andre_2).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 3, 28}),
    segmentationPoints(DSI::Andre_3).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 4, 25}),
    segmentationPoints(DSI::Andre_3).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 5, 22}),
    segmentationPoints(DSI::Andre_3).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 5, 15}),
    segmentationPoints(DSI::Andre_Squats_1).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 6, 16}),
    segmentationPoints(DSI::Andre_Squats_1).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 8, 9}),
    segmentationPoints(DSI::Andre_Squats_1).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 3, 28}),
    segmentationPoints(DSI::Andre_Squats_2).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 4, 22}),
    segmentationPoints(DSI::Andre_Squats_2).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 6, 3}),
    segmentationPoints(DSI::Andre_Squats_2).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 5, 9}),
    segmentationPoints(DSI::Lucas_1).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 6, 1}),
    segmentationPoints(DSI::Lucas_1).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 6, 21}),
    segmentationPoints(DSI::Lucas_1).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 4, 26}),
    segmentationPoints(DSI::Lucas_2).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 5, 15}),
    segmentationPoints(DSI::Lucas_2).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 6, 11}),
    segmentationPoints(DSI::Lucas_2).at(2));

  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 2, 24}),
    segmentationPoints(DSI::Lucas_3).at(0));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 3, 15}),
    segmentationPoints(DSI::Lucas_3).at(1));
  EXPECT_EQ(
    (cm::ManualSegmentationPoint{0, 0, 4, 14}),
    segmentationPoints(DSI::Lucas_3).at(2));
#undef DSI
}

TEST(ManualSegmentationPoint, shouldPrint)
{
  std::ostringstream oss{};
  oss.imbue(std::locale::classic());

  const cm::ManualSegmentationPoint manualSegmentationPoint{0, 1, 10, 29};
  oss << manualSegmentationPoint;
  const std::string str{oss.str()};

  using namespace std::string_literals;

  EXPECT_EQ(
    "ManualSegmentationPoint{\"hour\": 0, \"minute\": 1, \"second\": 10, \"frame\": 29}"s,
    str);
}
