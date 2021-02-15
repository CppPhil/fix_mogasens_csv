# mogasens_csv
This git repository contains the source code developed for my bachelor's thesis called "Algorithmic activity detection based on motion and position sensors."

TODO: TABLE OF CONTENTS

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
sudo apt-get install -y gcc make perl build-essential dkms dpkg-dev fakeroot g++ g++-9 libalgorithm-diff-perl libalgorithm-diff-xs-perl libalgorithm-merge-perl libdouble-conversion3 libfakeroot libgsoap-2.8.91 liblzf1 libpcre2-16-0 libstdc++-9-dev git libssl-dev ruby-full python3 python3-pip
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
(Source: https://askubuntu.com/a/865294)
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

TODO: Note about the commented out python stuff


TODO: Parameters of the bash scripts.?

TODO: Note about the MATLAB stuff and how it has created the preprocessed data sets.

TODO: Overview of files / directories etc. (all the stuff, what's interesting, what files to look at etc.)
