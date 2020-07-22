#!/bin/bash

catch_errors() {
  printf "\ncreate_mike_example.sh failed!\n" >&2
  exit 1
}

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd) 

cd "$DIR"

./clean.sh
./build.sh
./fix_csvs.sh

readonly PLOTTER="./python/mogasens_plotter/main.py"
readonly FILE="./resources/Mike/2020-07-02_14.07.33_out.csv"
readonly BELLY_SENSOR=770
readonly IMU=accelerometer

# Generate the example image with no filter.
$PLOTTER --no-moving_average_filter $FILE $BELLY_SENSOR $IMU 0

moving_average_filter_sample_counts=(
  25
  50
  100
  250
  500
)

# Generate the example image with the filters applied.
for moving_average_filter_sample_count in "${moving_average_filter_sample_counts[@]}"; do
  $PLOTTER --moving_average_filter $FILE $BELLY_SENSOR $IMU $moving_average_filter_sample_count
done

for image in ./resources/Mike/*.png; do
  if ! [[ "$image" == *3.png ]]; then
    rm "$image"
  fi
done

cd "$PREV_DIR"
exit 0

