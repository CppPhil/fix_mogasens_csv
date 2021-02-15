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

TODO: HERE

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


TODO: Applications

TODO: Overview of files / directories etc.
