#!/bin/bash

catch_errors() {
  printf "\ncompare_segmentation.sh failed!\n" >&2
  exit 1
}

trap catch_errors ERR;

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd)

cd "$DIR"

./build/confusion_matrix/confusion_matrix_app

cd "$PREV_DIR"
exit 0

