# OBJECT LIBRARY
set(LIB_NAME csv_lib)

set(LIB_HEADERS
    include/channel.hpp
    include/column.hpp
    include/data_point.hpp
    include/data_set.hpp
    include/error.hpp
    include/exception.hpp
    include/read_csv_file.hpp
    include/sensor.hpp
    include/to_string.hpp
)

set(LIB_SOURCES
    src/channel.cpp
    src/data_point.cpp
    src/data_set.cpp
    src/error.cpp
    src/expection.cpp
    src/read_csv_file.cpp
    src/sensor.cpp
)

add_library(${LIB_NAME} OBJECT ${LIB_HEADERS} ${LIB_SOURCES})

target_link_libraries(${LIB_NAME} PUBLIC Threads::Threads fmt::fmt)

target_include_directories(
  ${LIB_NAME}
  PUBLIC
  ${mogasens_csv_SOURCE_DIR}/csv_lib/include
  ${mogasens_csv_SOURCE_DIR}/external/philslib/include
  ${mogasens_csv_SOURCE_DIR}/external/csv-parser/single_include
  ${mogasens_csv_SOURCE_DIR}/external/tl/include
  ${mogasens_csv_SOURCE_DIR}/external/expected/include
  ${mogasens_csv_SOURCE_DIR}/external/optional/include
)

add_subdirectory(test)

