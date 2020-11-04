#!/bin/bash

catch_errors() {
  printf "\ngenerate_documentation.sh failed!\n" >&2
  exit 1
}

trap catch_errors ERR;

# Directory containing this bash script
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd)

generate_cpp_documentation() {
  doxygen

  cd documentation/latex
  make

  cd "$DIR"

  mv documentation/latex/refman.pdf documentation/cpp_documentation.pdf 

  rm -rf documentation/latex
}

cd "$DIR"

generate_cpp_documentation

cd "$PREV_DIR"

exit 0

