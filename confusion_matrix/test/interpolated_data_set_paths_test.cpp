#include <string>

#include "gtest/gtest.h"

#include <pl/algo/ranged_algorithms.hpp>

#include "interpolated_data_set_paths.hpp"

TEST(interpolatedDataSetPaths, shouldFetchPaths)
{
  using namespace std::literals::string_literals;

  std::vector<cl::fs::Path> expectedPaths{
    "resources/preprocessed/Interpolated/2020-07-02_11.17.39_Belly.csv"s,
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
    "resources/preprocessed/Interpolated-Revised/Jan_liegestuetzen1_13_10_2020_edited_Belly.csv"s,
    "resources/preprocessed/Interpolated-Revised/Jan_liegestuetzen1_13_10_2020_edited_Chest.csv"s,
    "resources/preprocessed/Interpolated-Revised/Jan_liegestuetzen1_13_10_2020_edited_LeftArm.csv"s,
    "resources/preprocessed/Interpolated-Revised/Jan_liegestuetzen1_13_10_2020_edited_RightArm.csv"s,
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
    "resources/preprocessed/Interpolated/Lukas_liegestuetzen3_13_10_2020_RightArm.csv"s};

  std::vector<cl::fs::Path> actual{cm::interpolatedDataSetPaths()};

  pl::algo::sort(expectedPaths);
  pl::algo::sort(actual);

  EXPECT_EQ(expectedPaths, actual);
}
