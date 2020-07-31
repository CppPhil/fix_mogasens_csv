#!/bin/bash

catch_errors() {
  printf "\nlint.sh failed!\n" >&2
  exit 1
}

trap catch_errors ERR;

# Directory containing this bash script.
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd)

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

run_rubocop "$DIR/ruby"

cd $PREV_DIR
exit 0

