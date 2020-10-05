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

cd "$DIR"

./build.sh
./fix_csvs.sh
./filter.sh --filter_sample_count=$FILTER_SAMPLE_COUNT

for csv_file in "${CSV_FILES[@]}"; do
  ./segment.sh --skip_window=true \
               --delete_too_close=true \
               --delete_low_variance=true \
               --image_format=png \
               --csv_file_path=$csv_file \
               --sensor=770 \
               --imu=accelerometer \
               --segmentation_kind=both \
               --window_size=51
done

cd "$PREV_DIR"
exit 0

