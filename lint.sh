#!/bin/bash

catch_errors() {
  printf "\nlint.sh failed!\n" >&2
  exit 1
}

trap catch_errors ERR;

# Directory containing this bash script.
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd)

clang_tidy() {
  if [ "$(which clang-tidy)" == "" ]; then
    printf "clang-tidy was not found, skipping.\n" >&2
    return
  fi
  
  ./python/run-clang-tidy.py -p build -j $(nproc) -format -quiet
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

./python/fix_compile_commands.py
clang_tidy "$DIR/feature_extraction"
clang_tidy "$DIR/fix_csv"
clang_tidy "$DIR/csv_lib"
run_rubocop "$DIR/ruby"

cd $PREV_DIR
exit 0

