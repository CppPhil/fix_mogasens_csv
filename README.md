# mogasens_csv
This git repository contains the source code developed for my bachelor's thesis called "Algorithmic activity detection based on motion and position sensors."

## Table of contents
- [mogasens_csv](#mogasens_csv)
  - [Table of contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Supported operating systems](#supported-operating-systems)
  - [Prerequisites](#prerequisites)
    - [apt-get packages](#apt-get-packages)
    - [Pip3 packages](#pip3-packages)
    - [CMake 3.19.5](#cmake-3195)
  - [Cloning](#cloning)
  - [Building](#building)
  - [Bash scripts](#bash-scripts)
    - [compare_segmentation.sh](#compare_segmentationsh)
    - [confusion_matrix.sh](#confusion_matrixsh)
    - [count.sh](#countsh)
    - [filter.sh](#filtersh)
    - [fix_csvs.sh](#fix_csvssh)
    - [generate_images.sh](#generate_imagessh)
    - [preprocessed_segment.sh](#preprocessed_segmentsh)
    - [segment_all.sh](#segment_allsh)
    - [segment.sh](#segmentsh)
  - [Generating images including segmentation points](#generating-images-including-segmentation-points)
  - [MATLAB](#matlab)
  - [Directories and files of interest](#directories-and-files-of-interest)
  - [Test person naming](#test-person-naming)
  - [Unit tests](#unit-tests)

## Introduction
The goal was to devise an algorithm of low runtime complexity that can segment accelerometer signals
of repetitive sports exercises such as push-ups.
The algorithm was written in Python and is located in `python/modules/segmentation_points.py`.
Several other utilities were written to evaluate the results as well as for other research purposes.
Most of these utilities were writen in C++.
The code that was written is stored in this git repository.
Bash scripts serve as entry points to the command line applications developed.
A substantial amount of so called glue code is written in Ruby, stored in the `ruby` directory.

## Supported operating systems
* GNU/Linux
* Microsoft Windows (experimental)

The software has been developed and tested on Ubuntu 20.04.
Support for Microsoft Windows is to be considered experimental at best.
If you're on Microsoft Windows you may want to try to follow the instructions provided in `windows_manual.pdf`.

## Prerequisites
Building and running the project's executables has several prerequisites on Ubuntu 20.04.

### apt-get packages
The following command installs the required apt-get packages.
```text
sudo apt-get install -y gcc make perl build-essential dkms dpkg-dev g++ g++-9 libalgorithm-diff-perl libalgorithm-diff-xs-perl libdouble-conversion3 libgsoap-2.8.91 liblzf1 libpcre2-16-0 libstdc++-9-dev git libssl-dev ruby-full python3 python3-pip
```

### Pip3 packages
The following command installs the required pip3 packages.
```text
pip3 install numpy scipy matplotlib ipython jupyter pandas sympy nose
```

### CMake 3.19.5
If you have some older CMake release installed through your package manager, you'll first have to uninstall that using something like `sudo apt remove --purge --auto-remove cmake`.
Install CMake 3.19.5 using the following executive summary.
```text
version=3.19
build=5
mkdir ~/temp
cd ~/temp
wget https://cmake.org/files/v$version/cmake-$version.$build.tar.gz
tar -xzvf cmake-$version.$build.tar.gz
cd cmake-$version.$build/
./bootstrap
make -j$(nproc)
sudo make install
```
(Source: <https://askubuntu.com/a/865294>)
Newer releases of CMake should also work.


## Cloning
To clone this git repository use `git clone --recursive https://github.com/CppPhil/mogasens_csv.git`.

## Building
To build everything invoke `build.sh` using a bash shell (e.g., `./build.sh`).
The `build.sh` bash script supports multiple options as shown in the following.
```text
  build.sh [OPTIONS] Builds the project using the supplied options.

  EXAMPLE:
    build.sh --build_type=Debug --verbose=true

  OPTIONS:
  -h
    --help                this help text

  --build_type=BUILD_TYPE The build type to use (Debug | Release)

  --verbose=BOOLEAN       Whether or not to use verbose output when invoking make (true | false)
```


## Bash scripts
The following tables shows the most interesting bash scripts.
For most bash scripts an experimental Microsoft Windows variant using the `.bat` file extension exists.
Most scripts will output usage instructions given the `--help` flag (e.g., `./script.sh --help`).

| Bash script             | Description | Notes |
|-------------------------|-------------|-------|
| compare_segmentation.sh | Reads the log files from segmentation\_comparison/logs and compares the number of segmentation points found for the preprocessed CSV files with the number of expected segmentation results. The results are written to segmentation\_comparison/out.csv and segmentation\_comparison/out.txt | Requires that segment\_all.sh has been run before.      |
| confusion_matrix.sh     | Creates confusion matrices for various algorithm configurations to determine the accuracy of the segmentation algorithm given the different configurations by comparing the segmentation points that were algorithmically determined with a ground truth.            | Does not support any command line parameters      |
| count.sh                | Determines the number of data points above / below the IMU thresholds. Also determines which channels are not relevant for which sensors in which data sets.            | If the `--filter_sample_count` option is omitted, no moving average filter will be used. Requires that fix\_csvs.sh has been run before.     |
| filter.sh               | Applies a moving average filter to the CSV data sets. The resulting data sets are written to the Felix, Marcelle, and Mike subdirectories of the resources directory.           | Requires that fix\_csvs.sh has been run before.       |
| fix_csvs.sh             | Preprocesses the raw CSV files.            | The results are written to the Felix, Marcelle, and Mike subdirectories of the resources directory.      |
| generate_images.sh      | Creates images of the CSV files, optionally applying a moving average filter.            | The images are written to the subdirectories of the resources directory.      |
| preprocessed_segment.sh | Segments a CSV file that has been preprocessed (i.e., a CSV file from one of the subdirectories of the resources/preprocessed directory).            | The results are written to stdout and stderr. Specifically, the segmentation points themselves are written to stderr.      |
| segment_all.sh          | Segments all the preprocessed CSV files using various configurations and writes the results to log files.            | The segmentation results are writen to log files in segmentation\_comparison/logs      |
| segment.sh              | Old segment script to segment one of the CSV files generated by fix\_csvs.sh.            | Requires fix\_csvs.sh to have been run before.      |

### compare_segmentation.sh
```text
Usage: compare_segmentation [options]
        --build_type=BUILD_TYPE      Debug | Release
        --compiler=COMPILER          MinGW | MSVC
        --mode=MODE                  The mode to use (AllDataSets | AllPushUps | PushUps250Hz | PushUps200Hz | Squats)
    -h, --help                       Show this message
```
Both the build\_type and compiler options can be ignored on GNU/Linux.
The mode options defaults to AllDataSets.
If mode is AllDataSets the optimal configuration for all data sets will be found by comparing the count of segmentation points.
If mode is AllPushUps the optimal configuration for all the push-ups will be found instead.
If mode is PushUps250Hz the optimal configuration for the push-ups sampled at 250 Hz will be found.
If mode is PushUps200Hz the optimal configuration for the push-ups sampled at 200 Hz will be found.
If mode is Squats the optimal configuration for the squats will be found.
Note that compare\_segmentation.sh will not considered the placement of the segmentation points, merely the counts are compared to the expected count.
The compare\_segmentation application is written in C++ and the source code is located in the compare\_segmentation directory.
Results are written to the segmentation\_comparison directory.

### confusion_matrix.sh
This application does not support any command line arguments.
It is written in C++ and the source code is located in the confusion\_matrix directory.
The result that's written to stdout is also written to `output.txt`.

### count.sh
```text
Usage: count [options]
        --build_type=BUILD_TYPE      Debug | Release
        --compiler=COMPILER          MinGW | MSVC
        --filter_sample_count=SAMPLE_COUNT
                                     Filter count for the moving average filter
    -h, --help                       Show this message
```
The build\_type and compiler options are not used for GNU/Linux.
The filter\_sample\_count option is the count of data points that shall be averaged together using the
moving average filter. This option can be omitted or supplied with a value of 0 to disable the use
of the moving average filter for the counting.
This application is written in C++ and the sources are located in the counting directory.

### filter.sh
```text
Usage: filter [options]
        --filter_sample_count=SAMPLE_COUNT
                                     Filter count for the moving average filter
    -h, --help                       Show this message
```
The filter\_sample\_count option is the count of data points that shall be averaged together using the
moving average filter.
This application is written in Python, see `python/filter.py`.

### fix_csvs.sh
```text
Usage: fix_csvs [options]
        --build_type=BUILD_TYPE      Debug | Release
        --compiler=COMPILER          MinGW | MSVC
    -h, --help                       Show this message
```
The build\_type and compiler options are not used for GNU/Linux.
This application is written in C++ and the sources are located in the fix\_csv directory.

### generate_images.sh
```text
Usage: generate_images [options]
        --filter_sample_count=SAMPLE_COUNT
                                     Filter count for the moving average filter
        --image_format=IMAGE_FORMAT  The image format to use e.g. svg; defaults to png
    -h, --help                       Show this message
```
The filter\_sample\_count option is the count of data points that shall be averaged together using the
moving average filter.
The image\_format parameter can be set to svg to generate svg images instead of png images.
If it is omitted png images will be generated.
The Ruby script `ruby/generate_images.rb` calls the appropriate Python scripts for all of the CSV files.

### preprocessed_segment.sh
```text
Usage: preprocessed_segment [options]
        --skip_window=BOOLEAN        Whether to skip over the remainder of the window when a segmentation point is found
        --delete_too_close=BOOLEAN   Whether to delete too close segmentation points
        --delete_low_variance=BOOLEAN
                                     Whether to delete segments with too low variance
        --image_format=IMAGE_FORMAT  The image format to use e.g. svg; defaults to png
        --csv_file_path=PATH         Path to the CSV file to segment.
        --imu=IMU                    The IMU to use (accelerometer | gyroscope)
        --segmentation_kind=KIND     The segmentation kind to use (min | max | both)
        --window_size=WINDOW_SIZE    The window size to use for segmenting
        --filter=FILTER              average or butterworth
    -h, --help                       Show this message
```
Example invocation from this mogasens\_csv root directory:
```text
./preprocessed_segment.sh --skip_window=false --delete_too_close=false --delete_low_variance=false --image_format=svg --csv_file_path=resources/preprocessed/Interpolated/Lukas_liegestuetzen1_13_10_2020_Chest.csv --imu=accelerometer --segmentation_kind=min --window_size=451 --filter=average
```
Note that only preprocessed CSV files can be used with this bash script (i.e., those located in subdirectories of resources/preprocessed).
This application is written in Python, see `python/preprocessed_segment.py`.

### segment_all.sh
```text
Usage: segment_all [options]
        --skip_existing=BOOLEAN      Whether to skip already existing log files (defaults to true)
    -h, --help                       Show this message
```
If you want to regenerate the log files generated by segment\_all set the `--skip_existing` option to false.
This application is written in Ruby, see `ruby/segment_all.rb`.

### segment.sh
```text
Usage: segment [options]
        --skip_window=BOOLEAN        Whether to skip over the remainder of the window when a segmentation point is found
        --delete_too_close=BOOLEAN   Whether to delete too close segmentation points
        --delete_low_variance=BOOLEAN
                                     Whether to delete segments with too low variance
        --image_format=IMAGE_FORMAT  The image format to use e.g. svg; defaults to png
        --csv_file_path=PATH         Path to the CSV file to segment.
        --sensor=SENSOR              The sensor to use for segmenting (769 | 770 | 771 | 772)
        --imu=IMU                    The IMU to use (accelerometer | gyroscope)
        --segmentation_kind=KIND     The segmentation kind to use (min | max | both)
        --window_size=WINDOW_SIZE    The window size to use for segmenting
    -h, --help                       Show this message
```
Example invocation from this mogasens\_csv root directory:
```text
./segment.sh --skip_window=false --delete_too_close=false --delete_low_variance=false --image_format=svg --csv_file_path=resources/Felix/2020-07-02_11.17.39_out_avg_filter_125.csv --sensor=770 --imu=accelerometer --segmentation_kind=min --window_size=451
```
Note that this command requires that `./fix_csvs.sh` and `./filter.sh --filter_sample_count=125` has been run before.
The sensors (769, 770, 771, 772) are the ExtractIDs from the CSV files.
The following table shows the mapping of these ExtractIDs to their sensor names.

| ExtractID | Sensor              |
|-----------|---------------------|
| 769       | left arm            |
| 770       | abdomen (aka belly) |
| 771       | right arm           |
| 772       | chest               |


## Generating images including segmentation points
In order to generate images that include segmentation points one can edit the `python/preprocessed_segment.py` file for the preprocessed CSV files or the `python/segment.py` file.
To do so remove the `#` characters that comment out lines 370 to 378 in `python/preprocessed_segment.py` or
lines 273 to 289 in `python/segment.py`.
Then re-run a script that generates segmentation points (e.g., `segment_all.sh --skip_existing=false`).

## MATLAB
The preprocessed data sets (i.e., the CSV files in the subdirectories of the resources/preprocessed directory)
were created using a MATLAB application that was developed by André Jeworutzki.
The git repository is at <https://git.csti.haw-hamburg.de/mogasens/dataanalyzer> (not publicly available as of the 16th of February 2021)

## Directories and files of interest

| Item                                                                | Description |
|-------------------------------------------------                    |-------------|
| confusion\_matrix/data/segmentation\_points\_imported\_from\_python | Contains log files of the segmentation points that were algorithmically determined in hardware timestamps. The log files are named according to the configuration that was used.
| confusion\_matrix/data/manual\_segmentation.csv                     | Contains the manually determined segmentation points created by examining the videos that were recorded. |
| output.txt                                                          | Output of the confusion\_matrix application. Line 12815 contains the optimal configuration used in the thesis according to the confusion matrices created. |
| compare_segmentation/include/data\_set\_info.hpp                    | Contains the repetitions as counted by the thesis author. |
| count\_comparison                                                   | Directory containing text files of the output of running compare\_counts.sh. The files show which data points are above / below the allowable thresholds and which channels are not relevant, depending on the filter sample count setting of the moving average filter used. |
| documentation                                                       | contains Doxygen generated documentation of the C++ source code. |
| resources/Felix                                                     | The raw CSV files of test person 1 |
| resources/Marcelle                                                  | The raw CSV files of test person 3 |
| resources/Mike                                                      | The raw CSV files of test person 2 |
| resources/preprocessed/Interpolated                                 | The CSV files that have been preprocessed using MATLAB. |
| resources/preprocessed/Interpolated-Revised                         | A revision of the first data set of push-ups of test person 4, this should be used instead of the Jan\_liegestuetzen1 files in resources/preprocessed/Interpolated |
| segmentation\_comparison                                            | The out.csv and out.txt files contain the output of the compare\_segmentation application. Line 530 shows the optimal configuration according to the distance score. |
| segmentatio\_comparison/logs                                        | Contains log files containing the counts of segmentation points that were algorithmically determined using the segmentation algorithm written in Python. The log files are named according to the configuration that was used to generate them. |

## Test person naming
The test persons are referred to numerically in the thesis text.
The table below shows the corresponding mapping.

| Numeric name  | Name                                  |
|---------------|---------------------------------------|
| Test person 1 | Felix                                 |
| Test person 2 | Mike                                  |
| Test person 3 | Marcelle (sometimes spelled as Marsi) |
| Test person 4 | Jan                                   |
| Test person 5 | André (sometimes spelled as Andre)    |
| Test person 6 | Lucas (sometimes spelled as Lukas)    |


## Unit tests
The unit tests can be run using `./test.sh`.
