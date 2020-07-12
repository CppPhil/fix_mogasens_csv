#!/bin/bash

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd)

cd $DIR

rm -rf ./build
rm -rf ./Testing
rm -f ./CMakeLists.txt.user
rm -f ./resources/Felix/2020-07-02_11.17.39_out.csv
rm -f ./resources/Felix/2020-07-02_12.50.00_out.csv
rm -f ./resources/Felix/2020-07-02_13.00.09_out.csv
rm -f ./resources/Marcelle/2020-07-02_14.59.59_out.csv
rm -f ./resources/Marcelle/2020-07-02_15.13.22_out.csv
rm -f ./resources/Marcelle/2020-07-02_15.31.36_out.csv
rm -f ./resources/Marcelle/2020-07-02_15.39.22_out.csv
rm -f ./resources/Mike/2020-07-02_14.07.33_out.csv
rm -f ./resources/Mike/2020-07-02_14.14.32_out.csv
rm -f ./resources/Mike/2020-07-02_14.20.28_out.csv
rm -f ./resources/Mike/2020-07-02_14.38.40_out.csv
rm -f ./resources/Felix/*.png
rm -f ./resources/Marcelle/*.png
rm -f ./resources/Mike/*.png
rm -f ./log.txt

cd $PREV_DIR

exit 0

