#!/bin/bash

# Directory containing this bash script.
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd)

clang_tidy() {
  if [ "$(which clang-tidy)" == "" ]; then
    printf "clang-tidy was not found, skipping.\n" >&2
    return
  fi
  
  ./run-clang-tidy.py -p build -j $(nproc) -format
}

run_rubocop() {
  ruby_dir="$1"

  if [ "$(which rubocop)" == "" ]; then
    printf "rubocop was not found, skipping.\n" >&2
    return
  fi

  echo "Running rubocop"
  cd "$ruby_dir" && rubocop -a
  cd "$DIR"
  printf "Finished running rubocop.\n\n"
}

cd $DIR

clang_tidy "$DIR/feature_extraction"
clang_tidy "$DIR/fix_csv"
run_rubocop "$DIR/ruby"

cd $PREV_DIR
exit 0

