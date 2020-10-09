#!/bin/bash

catch_errors() {
  printf "\nsegment_all_old.sh failed!\n" >&2
  exit 1
}

trap catch_errors ERR;

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd)

readonly RESOURCE_DIR="resources"
readonly FELIX_DIR="$RESOURCE_DIR/Felix"
readonly MARCELLE_DIR="$RESOURCE_DIR/Marcelle"
readonly MIKE_DIR="$RESOURCE_DIR/Mike"

readonly FILTER_SAMPLE_COUNT=125

readonly SUFFIX="_out_avg_filter_$FILTER_SAMPLE_COUNT.csv"

readonly CSV_FILES=(
  "$FELIX_DIR/2020-07-02_11.17.39$SUFFIX"
  "$FELIX_DIR/2020-07-02_12.50.00$SUFFIX"
  "$FELIX_DIR/2020-07-02_13.00.09$SUFFIX"
  "$MARCELLE_DIR/2020-07-02_14.59.59$SUFFIX"
  "$MARCELLE_DIR/2020-07-02_15.13.22$SUFFIX"
  "$MARCELLE_DIR/2020-07-02_15.31.36$SUFFIX"
  "$MIKE_DIR/2020-07-02_14.07.33$SUFFIX"
  "$MIKE_DIR/2020-07-02_14.14.32$SUFFIX"
  "$MIKE_DIR/2020-07-02_14.20.28$SUFFIX"
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
  "51"
)

readonly SENSORS=(
  "769"
  "770"
  "771"
  "772"
)

cd "$DIR"

readonly LOG_DIR="$DIR/segmentation_comparison/logs"

./build.sh
./fix_csvs.sh
./filter.sh --filter_sample_count=$FILTER_SAMPLE_COUNT

echo "segment_all_old.sh: Starting."

rm -f "$LOG_DIR/*.log"

for skip_window_option in "${SKIP_WINDOW_OPTIONS[@]}"; do
  for delete_too_close_option in "${DELETE_TOO_CLOSE_OPTIONS[@]}"; do
    for delete_low_variance_option in "${DELETE_LOW_VARIANCE_OPTIONS[@]}"; do
      for segmentation_kind in "${SEGMENTATION_KINDS[@]}"; do
        for window_size in "${WINDOW_SIZES[@]}"; do
          for sensor in "${SENSORS[@]}"; do
            for csv_file in "${CSV_FILES[@]}"; do
              ./segment.sh \
                --skip_window="$skip_window_option" \
                --delete_too_close="$delete_too_close_option" \
                --delete_low_variance="$delete_low_variance_option" \
                --image_format=png \
                --csv_file_path="$csv_file" \
                --sensor="$sensor" \
                --imu=accelerometer \
                --segmentation_kind="$segmentation_kind" \
                --window_size="$window_size" \
                  >> "$LOG_DIR/old/skip_window-${skip_window_option}_delete_too_close-${delete_too_close_option}_delete_low_variance-${delete_low_variance_option}_sensor-${sensor}_kind-${segmentation_kind}_window-${window_size}.log"
            done
          done
        done
      done
    done
  done
done

echo "DONE."

cd "$PREV_DIR"
exit 0

