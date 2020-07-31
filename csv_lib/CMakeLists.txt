# STATIC LIBRARY
set(LIB_NAME csv_lib)

set(LIB_HEADERS
    include/cl/fs/directory.hpp
    include/cl/fs/file.hpp
    include/cl/fs/file_stream.hpp
    include/cl/fs/path.hpp
    include/cl/fs/windows.hpp
    include/cl/channel.hpp
    include/cl/column.hpp
    include/cl/data_point.hpp
    include/cl/data_set.hpp
    include/cl/dos2unix.hpp
    include/cl/error.hpp
    include/cl/exception.hpp
    include/cl/read_csv_file.hpp
    include/cl/s2n.hpp
    include/cl/sensor.hpp
    include/cl/to_string.hpp
    include/cl/unreachable.hpp)

set(LIB_SOURCES
    src/cl/fs/directory.cpp
    src/cl/fs/file.cpp
    src/cl/fs/file_stream.cpp
    src/cl/fs/path.cpp
    src/cl/fs/windows.cpp
    src/cl/channel.cpp
    src/cl/data_point.cpp
    src/cl/data_set.cpp
    src/cl/dos2unix.cpp
    src/cl/error.cpp
    src/cl/exception.cpp
    src/cl/read_csv_file.cpp
    src/cl/sensor.cpp)

add_library(${LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})

target_link_libraries(${LIB_NAME} PUBLIC Threads::Threads fmt::fmt
                                         CppPhil::philslib dlib::dlib)

if(WIN32)
  target_link_libraries(${LIB_NAME} PUBLIC Kernel32.lib)
endif()

target_include_directories(${LIB_NAME}
                           PUBLIC ${mogasens_csv_SOURCE_DIR}/csv_lib/include)

target_include_directories(
  ${LIB_NAME}
  PUBLIC ${mogasens_csv_SOURCE_DIR}/external/csv-parser/single_include
         ${mogasens_csv_SOURCE_DIR}/external/tl/include
         ${mogasens_csv_SOURCE_DIR}/external/expected/include
         ${mogasens_csv_SOURCE_DIR}/external/optional/include)

add_subdirectory(test)
