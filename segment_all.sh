#!/bin/bash

catch_errors() {
  printf "\nsegment_all.sh failed!\n" >&2
  exit 1
}

trap catch_errors ERR;

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd)

readonly RESOURCE_DIR="resources"

readonly PREPROCESSED_DIR="$RESOURCE_DIR/preprocessed"

readonly CSV_FILES=(
  "$PREPROCESSED_DIR/2020-07-02_11.17.39_Belly.csv"
  "$PREPROCESSED_DIR/2020-07-02_11.17.39_Chest.csv"
  "$PREPROCESSED_DIR/2020-07-02_11.17.39_LeftArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_11.17.39_RightArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_12.50.00_Belly.csv"
  "$PREPROCESSED_DIR/2020-07-02_12.50.00_Chest.csv"
  "$PREPROCESSED_DIR/2020-07-02_12.50.00_LeftArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_12.50.00_RightArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_13.00.09_Belly.csv"
  "$PREPROCESSED_DIR/2020-07-02_13.00.09_Chest.csv"
  "$PREPROCESSED_DIR/2020-07-02_13.00.09_LeftArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_13.00.09_RightArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.07.33_Belly.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.07.33_Chest.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.07.33_LeftArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.07.33_RightArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.14.32_Belly.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.14.32_Chest.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.14.32_LeftArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.14.32_RightArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.20.28_Belly.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.20.28_Chest.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.20.28_LeftArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.20.28_RightArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.38.40 (FalschausfБhrung)_Belly.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.38.40 (FalschausfБhrung)_Chest.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.38.40 (FalschausfБhrung)_LeftArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.38.40 (FalschausfБhrung)_RightArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.59.59_Belly.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.59.59_Chest.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.59.59_LeftArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_14.59.59_RightArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_15.13.22_Belly.csv"
  "$PREPROCESSED_DIR/2020-07-02_15.13.22_Chest.csv"
  "$PREPROCESSED_DIR/2020-07-02_15.13.22_LeftArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_15.13.22_RightArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_15.31.36_Belly.csv"
  "$PREPROCESSED_DIR/2020-07-02_15.31.36_Chest.csv"
  "$PREPROCESSED_DIR/2020-07-02_15.31.36_LeftArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_15.31.36_RightArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_15.39.22 (FalschausfБhrung)_Belly.csv"
  "$PREPROCESSED_DIR/2020-07-02_15.39.22 (FalschausfБhrung)_Chest.csv"
  "$PREPROCESSED_DIR/2020-07-02_15.39.22 (FalschausfБhrung)_LeftArm.csv"
  "$PREPROCESSED_DIR/2020-07-02_15.39.22 (FalschausfБhrung)_RightArm.csv"
)

cd "$DIR"

for csv_file in "${CSV_FILES[@]}"; do
  ./preprocessed_segment.sh --skip_window=true \
                            --delete_too_close=true \
                            --image_format=png \
                            --csv_file_path="$csv_file" \
                            --imu=accelerometer \
                            --segmentation_kind=both \
                            --window_size=51
done

cd "$PREV_DIR"
exit 0

