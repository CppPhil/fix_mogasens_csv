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
readonly IMU=accelerometer

sensors=(
  769  # left arm
#  770 # belly
#  771 # right arm
#  772 # chest
)

# Generate the example image with no filter.
for sensor in "${sensors[@]}"; do
  $PLOTTER --no-moving_average_filter $FILE $sensor $IMU 0
done

# Generate the example image with the filters applied.
for sensor in "${sensors[@]}"; do
  for (( i=25; i<=500; i+=25 )); do
    $PLOTTER --moving_average_filter $FILE $sensor $IMU $i
  done
done

cd "$PREV_DIR"
exit 0

