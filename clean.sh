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
rm -f ./resources/Felix/*.svg
rm -f ./resources/Marcelle/*.svg
rm -f ./resources/Mike/*.svg
rm -f ./resources/Felix/*.png
rm -f ./resources/Marcelle/*.png
rm -f ./resources/Mike/*.png
rm -f ./resources/preprocessed/Interpolated/*.png
rm -f ./resources/preprocessed/Interpolated/*.svg
rm -f ./log.txt
rm -rf ./python/__pycache__
rm -rf ./python/modules/__pycache__

cd $PREV_DIR

exit 0

