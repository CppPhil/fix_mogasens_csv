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

readonly SKIP_WINDOW_OPTIONS=(
  "false"
  "true"
)

readonly DELETE_TOO_CLOSE_OPTIONS=(
  "false"
  "true"
)

readonly DELETE_LOW_VARIANCE_OPTIONS=(
  "false"
  "true"
)

readonly SEGMENTATION_KINDS=(
  "both"
  "max"
  "min"
)

readonly WINDOW_SIZES=(
  "101"
  "151"
  "201"
  "251"
  "51"
)

readonly FILTERS=(
  "average"
  "butterworth"
)

cd "$DIR"

readonly LOG_DIR="$DIR/segmentation_comparison/logs"

rm -f "$LOG_DIR/*log"

for skip_window_option in "${SKIP_WINDOW_OPTIONS[@]}"; do
  for delete_too_close_option in "${DELETE_TOO_CLOSE_OPTIONS[@]}"; do
    for delete_low_variance_option in "${DELETE_LOW_VARIANCE_OPTIONS[@]}"; do
      for segmentation_kind in "${SEGMENTATION_KINDS[@]}"; do
        for window_size in "${WINDOW_SIZES[@]}"; do
          for filter in "${FILTERS[@]}"; do
            for csv_file in "${CSV_FILES[@]}"; do
              ./preprocessed_segment.sh \
                --skip_window="$skip_window_option" \
                --delete_too_close="$delete_too_close_option" \
                --delete_low_variance="$delete_low_variance_option" \
                --image_format=png \
                --csv_file_path="$csv_file" \
                --imu=accelerometer \
                --segmentation_kind="$segmentation_kind" \
                --window_size="$window_size" \
                --filter="$filter" \
                  >> "$LOG_DIR/skip_window-${skip_window_option}_delete_too_close-${delete_too_close_option}_delete_low_variance-${delete_low_variance_option}_kind-${segmentation_kind}_window-${window_size}_filter-${filter}.log"
            done
          done
        done
      done
    done
  done
done

echo "Done."

cd "$PREV_DIR"
exit 0

