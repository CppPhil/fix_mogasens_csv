#!/bin/bash

catch_errors() {
  printf "\ncompare_counts.sh failed!\n" >&2
  exit 1
}

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd) 

cd "$DIR"

./clean.sh && ./build.sh && ./fix_csvs.sh

./count.sh > count_comparison/count_unfiltered.txt

rm -rf resources

git checkout resources

for (( i=25; i<=500; i+=25 )); do
  rm -rf resources

  git checkout resources
  ./fix_csvs.sh
  ./count.sh --filter_sample_count=$i > count_comparison/count_$i.txt
done

cd "$PREV_DIR"
exit 0

