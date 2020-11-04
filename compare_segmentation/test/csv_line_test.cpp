#include <array>

#include "gtest/gtest.h"

#include <pl/unreachable.hpp>

#include "csv_line.hpp"

namespace {
[[nodiscard]] std::vector<std::string> create(
  std::uint64_t sensorId,
  bool          isOld)
{
  return cs::CsvLineBuilder{}
    .skipWindow(true)
    .deleteTooClose(true)
    .deleteLowVariance(true)
    .kind(cs::SegmentationKind::Minima)
    .windowSize(151)
    .filter(cs::FilterKind::Butterworth)
    .dataSet("DATASET")
    .sensor(sensorId)
    .repetitions(55)
    .segmentationPoints(60)
    .isOld(isOld)
    .build();
}
} // namespace

TEST(CsvLine, shouldWork)
{
  static constexpr std::array<int, 4>  sensorIds   = {769, 770, 771, 772};
  static constexpr std::array<bool, 2> isOldValues = {false, true};

  for (int sensorId : sensorIds) {
    const std::string expectedSensor{[sensorId] {
      switch (sensorId) {
      case 769: return "LeftArm";
      case 770: return "Belly";
      case 771: return "RightArm";
      case 772: return "Chest";
      }
      PL_UNREACHABLE();
    }()};

    for (bool isOldValue : isOldValues) {
      const std::string expectedIsOldString{[isOldValue] {
        if (isOldValue) { return "old"; }
        return "preprocessed";
      }()};

      const std::vector<std::string> line{create(sensorId, isOldValue)};

      ASSERT_EQ(11U, line.size());

      for (int i{0}; i < 3; ++i) { EXPECT_EQ("true", line[i]); }

      EXPECT_EQ("Minima", line[3]);
      EXPECT_EQ("151", line[4]);
      EXPECT_EQ("Butterworth", line[5]);
      EXPECT_EQ("DATASET", line[6]);
      EXPECT_EQ(expectedSensor, line[7]);
      EXPECT_EQ("55", line[8]);
      EXPECT_EQ("60", line[9]);
      EXPECT_EQ(expectedIsOldString, line[10]);
    }
  }
}
