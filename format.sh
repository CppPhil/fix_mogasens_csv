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

  yapf -i --style='{based_on_style: pep8, indent_width: 2}' $python_src_file
}

cd $DIR

python_files=$(find "$DIR/python" -name '*.py')
for python_file in "${python_files[@]}"; do
  format_python "$python_file"
done

format_cmake $DIR/CMakeLists.txt

# Format the source files.
format "$DIR/compare_segmentation"
format "$DIR/confusion_matrix"
format "$DIR/counting"
format "$DIR/csv_lib"
format "$DIR/feature_extraction"
format "$DIR/fix_csv"

cd $PREV_DIR

exit 0

