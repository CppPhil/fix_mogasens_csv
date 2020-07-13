#!/bin/bash

### This script is used to run clang-format on the source files.

# Directory containing this bash script.
readonly DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

readonly PREV_DIR=$(pwd)

readonly DOT_CLANG_FORMAT=$DIR/.clang-format

delete_file_if_exists () {
  if [ -e $1 ]; then
    rm -f $1
  fi
}

format_cmake () {
  if [ "$(which cmake-format)" == "" ]; then
    printf "cmake-format was not found, skipping.\n" >&2
    return
  fi

  cmakelists_file=$1
  tmp_file="${cmakelists_file}.tmp"

  cmake-format $cmakelists_file > $tmp_file
  rm -f $cmakelists_file
  mv $tmp_file $cmakelists_file
}

format () {
  if [ "$(which clang-format)" == "" ]; then
    printf "clang-format was not found, skipping.\n" >&2
    return
  fi

  cd $1
  delete_file_if_exists ./.clang-format
  cp $DOT_CLANG_FORMAT ./.clang-format
  find -name '*.cpp' -o -name '*.hh' -o -name '*.hpp' -o -name '*.h' | xargs clang-format -i
  rm -f ./.clang-format
  
  file=$(find -name 'CMakeLists.txt')
  file_array=(${file})

  for f in "${file_array[@]}"; do
    format_cmake "$f"
  done
}

format_python() {
  python_src_file="$1"
  
  if [ "$(which yapf)" == "" ]; then
    printf "yapf was not found, skipping.\n" >&2
    return
  fi

  yapf -i --style='{based_on_style: pep8, indent_width: 4}' $python_src_file
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

run_rubocop "$DIR/ruby"

format_python $DIR/mogasens_plotter/main.py

format_cmake $DIR/CMakeLists.txt

# Format the source files.
format "$DIR/fix_csv"
format "$DIR/feature_extraction"

cd $PREV_DIR

exit 0

