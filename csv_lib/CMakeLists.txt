# STATIC LIBRARY
set(LIB_NAME csv_lib)

set(LIB_HEADERS
    include/cl/channel.hpp
    include/cl/column.hpp
    include/cl/data_point.hpp
    include/cl/data_set.hpp
    include/cl/error.hpp
    include/cl/exception.hpp
    include/cl/read_csv_file.hpp
    include/cl/s2n.hpp
    include/cl/sensor.hpp
    include/cl/to_string.hpp
    include/cl/unreachable.hpp)

set(LIB_SOURCES
    src/channel.cpp
    src/data_point.cpp
    src/data_set.cpp
    src/error.cpp
    src/exception.cpp
    src/read_csv_file.cpp
    src/sensor.cpp)

add_library(${LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})

target_link_libraries(${LIB_NAME} PUBLIC Threads::Threads fmt::fmt
                                         CppPhil::philslib dlib::dlib)

target_include_directories(${LIB_NAME}
                           INTERFACE ${mogasens_csv_SOURCE_DIR}/csv_lib/include)

target_include_directories(
  ${LIB_NAME}
  PUBLIC ${mogasens_csv_SOURCE_DIR}/external/csv-parser/single_include
         ${mogasens_csv_SOURCE_DIR}/external/tl/include
         ${mogasens_csv_SOURCE_DIR}/external/expected/include
         ${mogasens_csv_SOURCE_DIR}/external/optional/include)

target_include_directories(
  ${LIB_NAME} PRIVATE ${mogasens_csv_SOURCE_DIR}/csv_lib/include/cl)

add_subdirectory(test)
