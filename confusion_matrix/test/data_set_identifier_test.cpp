#include <array>

#include "gtest/gtest.h"

#include <cl/fs/path.hpp>

#include "data_set_identifier.hpp"

TEST(DataSetIdentifier, shouldConvertPaths)
{
  using namespace std::string_literals;

  const std::array<cl::fs::Path, 80> paths{
    {"resources/preprocessed/Interpolated/2020-07-02_11.17.39_Belly.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_11.17.39_Chest.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_11.17.39_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_11.17.39_RightArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_12.50.00_Belly.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_12.50.00_Chest.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_12.50.00_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_12.50.00_RightArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_13.00.09_Belly.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_13.00.09_Chest.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_13.00.09_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_13.00.09_RightArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.07.33_Belly.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.07.33_Chest.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.07.33_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.07.33_RightArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.14.32_Belly.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.14.32_Chest.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.14.32_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.14.32_RightArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.20.28_Belly.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.20.28_Chest.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.20.28_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.20.28_RightArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.59.59_Belly.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.59.59_Chest.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.59.59_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_14.59.59_RightArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_15.13.22_Belly.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_15.13.22_Chest.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_15.13.22_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_15.13.22_RightArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_15.31.36_Belly.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_15.31.36_Chest.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_15.31.36_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/2020-07-02_15.31.36_RightArm.csv"s,
     "resources/preprocessed/Interpolated/Andre_Squats2_13_10_2020_Belly.csv"s,
     "resources/preprocessed/Interpolated/Andre_Squats2_13_10_2020_Chest.csv"s,
     "resources/preprocessed/Interpolated/Andre_Squats2_13_10_2020_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/Andre_Squats2_13_10_2020_RightArm.csv"s,
     "resources/preprocessed/Interpolated/Andre_Squats_13_10_2020_Belly.csv"s,
     "resources/preprocessed/Interpolated/Andre_Squats_13_10_2020_Chest.csv"s,
     "resources/preprocessed/Interpolated/Andre_Squats_13_10_2020_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/Andre_Squats_13_10_2020_RightArm.csv"s,
     "resources/preprocessed/Interpolated/Andre_liegestuetzen1_13_10_2020_Belly.csv"s,
     "resources/preprocessed/Interpolated/Andre_liegestuetzen1_13_10_2020_Chest.csv"s,
     "resources/preprocessed/Interpolated/Andre_liegestuetzen1_13_10_2020_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/Andre_liegestuetzen1_13_10_2020_RightArm.csv"s,
     "resources/preprocessed/Interpolated/Andre_liegestuetzen2_13_10_2020_Belly.csv"s,
     "resources/preprocessed/Interpolated/Andre_liegestuetzen2_13_10_2020_Chest.csv"s,
     "resources/preprocessed/Interpolated/Andre_liegestuetzen2_13_10_2020_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/Andre_liegestuetzen2_13_10_2020_RightArm.csv"s,
     "resources/preprocessed/Interpolated/Andre_liegestuetzen3_13_10_2020_Belly.csv"s,
     "resources/preprocessed/Interpolated/Andre_liegestuetzen3_13_10_2020_Chest.csv"s,
     "resources/preprocessed/Interpolated/Andre_liegestuetzen3_13_10_2020_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/Andre_liegestuetzen3_13_10_2020_RightArm.csv"s,
     "resources/preprocessed/Interpolated/Jan_liegestuetzen1_13_10_2020_Belly.csv"s,
     "resources/preprocessed/Interpolated/Jan_liegestuetzen1_13_10_2020_Chest.csv"s,
     "resources/preprocessed/Interpolated/Jan_liegestuetzen1_13_10_2020_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/Jan_liegestuetzen1_13_10_2020_RightArm.csv"s,
     "resources/preprocessed/Interpolated/Jan_liegestuetzen2_13_10_2020_Belly.csv"s,
     "resources/preprocessed/Interpolated/Jan_liegestuetzen2_13_10_2020_Chest.csv"s,
     "resources/preprocessed/Interpolated/Jan_liegestuetzen2_13_10_2020_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/Jan_liegestuetzen2_13_10_2020_RightArm.csv"s,
     "resources/preprocessed/Interpolated/Jan_liegestuetzen3_13_10_2020_Belly.csv"s,
     "resources/preprocessed/Interpolated/Jan_liegestuetzen3_13_10_2020_Chest.csv"s,
     "resources/preprocessed/Interpolated/Jan_liegestuetzen3_13_10_2020_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/Jan_liegestuetzen3_13_10_2020_RightArm.csv"s,
     "resources/preprocessed/Interpolated/Lukas_liegestuetzen1_13_10_2020_Belly.csv"s,
     "resources/preprocessed/Interpolated/Lukas_liegestuetzen1_13_10_2020_Chest.csv"s,
     "resources/preprocessed/Interpolated/Lukas_liegestuetzen1_13_10_2020_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/Lukas_liegestuetzen1_13_10_2020_RightArm.csv"s,
     "resources/preprocessed/Interpolated/Lukas_liegestuetzen2_13_10_2020_Belly.csv"s,
     "resources/preprocessed/Interpolated/Lukas_liegestuetzen2_13_10_2020_Chest.csv"s,
     "resources/preprocessed/Interpolated/Lukas_liegestuetzen2_13_10_2020_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/Lukas_liegestuetzen2_13_10_2020_RightArm.csv"s,
     "resources/preprocessed/Interpolated/Lukas_liegestuetzen3_13_10_2020_Belly.csv"s,
     "resources/preprocessed/Interpolated/Lukas_liegestuetzen3_13_10_2020_Chest.csv"s,
     "resources/preprocessed/Interpolated/Lukas_liegestuetzen3_13_10_2020_LeftArm.csv"s,
     "resources/preprocessed/Interpolated/Lukas_liegestuetzen3_13_10_2020_RightArm.csv"s}};

#define DSI ::cm::DataSetIdentifier
  const std::array<cm::DataSetIdentifier, 80> expected{
    {DSI::Felix_11_17_39, DSI::Felix_11_17_39, DSI::Felix_11_17_39,
     DSI::Felix_11_17_39, DSI::Felix_12_50_00, DSI::Felix_12_50_00,
     DSI::Felix_12_50_00, DSI::Felix_12_50_00, DSI::Felix_13_00_09,
     DSI::Felix_13_00_09, DSI::Felix_13_00_09, DSI::Felix_13_00_09,
     DSI::Mike_14_07_33,  DSI::Mike_14_07_33,  DSI::Mike_14_07_33,
     DSI::Mike_14_07_33,  DSI::Mike_14_14_32,  DSI::Mike_14_14_32,
     DSI::Mike_14_14_32,  DSI::Mike_14_14_32,  DSI::Mike_14_20_28,
     DSI::Mike_14_20_28,  DSI::Mike_14_20_28,  DSI::Mike_14_20_28,
     DSI::Marsi_14_59_59, DSI::Marsi_14_59_59, DSI::Marsi_14_59_59,
     DSI::Marsi_14_59_59, DSI::Marsi_15_13_22, DSI::Marsi_15_13_22,
     DSI::Marsi_15_13_22, DSI::Marsi_15_13_22, DSI::Marsi_15_31_36,
     DSI::Marsi_15_31_36, DSI::Marsi_15_31_36, DSI::Marsi_15_31_36,
     DSI::Andre_Squats_2, DSI::Andre_Squats_2, DSI::Andre_Squats_2,
     DSI::Andre_Squats_2, DSI::Andre_Squats_1, DSI::Andre_Squats_1,
     DSI::Andre_Squats_1, DSI::Andre_Squats_1, DSI::Andre_1,
     DSI::Andre_1,        DSI::Andre_1,        DSI::Andre_1,
     DSI::Andre_2,        DSI::Andre_2,        DSI::Andre_2,
     DSI::Andre_2,        DSI::Andre_3,        DSI::Andre_3,
     DSI::Andre_3,        DSI::Andre_3,        DSI::Jan_1,
     DSI::Jan_1,          DSI::Jan_1,          DSI::Jan_1,
     DSI::Jan_2,          DSI::Jan_2,          DSI::Jan_2,
     DSI::Jan_2,          DSI::Jan_3,          DSI::Jan_3,
     DSI::Jan_3,          DSI::Jan_3,          DSI::Lucas_1,
     DSI::Lucas_1,        DSI::Lucas_1,        DSI::Lucas_1,
     DSI::Lucas_2,        DSI::Lucas_2,        DSI::Lucas_2,
     DSI::Lucas_2,        DSI::Lucas_3,        DSI::Lucas_3,
     DSI::Lucas_3,        DSI::Lucas_3}};

  ASSERT_EQ(expected.size(), paths.size());

  for (std::size_t i{0}; i < expected.size(); ++i) {
    EXPECT_EQ(expected[i], cm::toDataSetIdentifier(paths[i]));
  }

  // Test Interpolated-Revised
  EXPECT_EQ(
    DSI::Jan_1,
    cm::toDataSetIdentifier("resources/preprocessed/Interpolated-Revised/"
                            "Jan_liegestuetzen1_13_10_2020_edited_Belly.csv"));
  EXPECT_EQ(
    DSI::Jan_1,
    cm::toDataSetIdentifier("resources/preprocessed/Interpolated-Revised/"
                            "Jan_liegestuetzen1_13_10_2020_edited_Chest.csv"));
  EXPECT_EQ(
    DSI::Jan_1,
    cm::toDataSetIdentifier(
      "resources/preprocessed/Interpolated-Revised/"
      "Jan_liegestuetzen1_13_10_2020_edited_LeftArm.csv"));
  EXPECT_EQ(
    DSI::Jan_1,
    cm::toDataSetIdentifier(
      "resources/preprocessed/Interpolated-Revised/"
      "Jan_liegestuetzen1_13_10_2020_edited_RightArm.csv"));

#undef DSI
}
