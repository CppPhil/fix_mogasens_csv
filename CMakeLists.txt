include(FetchContent)

cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(mogasens_csv VERSION 1.0.0)

enable_testing()

if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    string(APPEND CMAKE_CXX_FLAGS "/W4")
  endif()

  string(
    APPEND
    CMAKE_CXX_FLAGS
    " /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS /D_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING /DNOMINMAX /D_UNICODE /DUNICODE"
  )

  string(APPEND CMAKE_CXX_FLAGS_DEBUG " /Od /DDEBUG_MODE")
  string(APPEND CMAKE_CXX_FLAGS_RELEASE " /O2 /DNDEBUG /DRELEASE_MODE")
else()
  string(
    APPEND
    CMAKE_CXX_FLAGS
    " -fwrapv -Werror -Wall -Wextra -Wstrict-aliasing -Wcast-align -Wmissing-include-dirs -Wpointer-arith -Wunreachable-code -Wunused -Wunused-parameter -Wno-uninitialized -Wno-deprecated-copy -Wno-aggressive-loop-optimizations -Wno-implicit-fallthrough"
  )

  string(APPEND CMAKE_CXX_FLAGS_DEBUG " -g -O0 -DDEBUG_MODE")
  string(APPEND CMAKE_CXX_FLAGS_RELEASE " -g -O3 -DNDEBUG -DRELEASE_MODE")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(DLIB_NO_GUI_SUPPORT
    ON
    CACHE BOOL
          "Enable this if you don't want to compile any of the dlib GUI code"
          FORCE)
set(DLIB_JPEG_SUPPORT
    OFF
    CACHE BOOL "Disable this if you don't want to link against libjpeg" FORCE)
set(DLIB_LINK_WITH_SQLITE3
    OFF
    CACHE BOOL "Disable this if you don't want to link against sqlite3" FORCE)
set(DLIB_USE_BLAS
    OFF
    CACHE BOOL "Disable this if you don't want to use a BLAS library" FORCE)
set(DLIB_USE_LAPACK
    OFF
    CACHE BOOL "Disable this if you don't want to use a LAPACK library" FORCE)
set(DLIB_USE_CUDA
    OFF
    CACHE BOOL "Disable this if you don't want to use NVIDIA CUDA" FORCE)
set(DLIB_PNG_SUPPORT
    OFF
    CACHE BOOL "Disable this if you don't want to link against libpng" FORCE)
set(DLIB_GIF_SUPPORT
    OFF
    CACHE BOOL "Disable this if you don't want to link against libgif" FORCE)
set(DLIB_USE_MKL_FFT
    OFF
    CACHE
      BOOL
      "Disable this is you don't want to use the MKL DFTI FFT implementation"
      FORCE)
FetchContent_Declare(
  dlib
  GIT_REPOSITORY https://github.com/davisking/dlib.git
  GIT_TAG v19.20)
FetchContent_MakeAvailable(dlib)

find_package(Threads REQUIRED)

add_subdirectory(external/fmtlib)
add_subdirectory(external/googletest)
add_subdirectory(external/scnlib)
add_subdirectory(external/philslib)

add_subdirectory(csv_lib)
add_subdirectory(fix_csv)
add_subdirectory(counting)
add_subdirectory(feature_extraction)
