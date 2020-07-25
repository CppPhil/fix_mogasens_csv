#!/bin/bash

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd)

cd $DIR

rm -rf ./build
rm -rf ./Testing
rm -f ./CMakeLists.txt.user
rm -f ./resources/Felix/*_out*.csv
rm -f ./resources/Marcelle/*_out*.csv
rm -f ./resources/Mike/*_out*.csv
rm -f ./resources/Felix/*.png
rm -f ./resources/Marcelle/*.png
rm -f ./resources/Mike/*.png
rm -f ./log.txt

cd $PREV_DIR

exit 0

