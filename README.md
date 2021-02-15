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

* git (`sudo apt-get install -y git`)
* svn


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

TODO: Overview
