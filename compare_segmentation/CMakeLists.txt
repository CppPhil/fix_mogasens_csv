# STATIC LIBRARY
set(LIB_NAME compare_segmentation_lib)

set(LIB_HEADERS
    include/csv_line.hpp
    include/data_set_info.hpp
    include/filter_kind.hpp
    include/log_files.hpp
    include/log_info.hpp
    include/log_line.hpp
    include/mode.hpp
    include/paths.hpp
    include/segmentation_kind.hpp)

set(LIB_SOURCES
    src/csv_line.cpp
    src/data_set_info.cpp
    src/filter_kind.cpp
    src/log_files.cpp
    src/log_info.cpp
    src/log_line.cpp
    src/mode.cpp
    src/segmentation_kind.cpp)

add_library(${LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})

target_link_libraries(${LIB_NAME} PUBLIC csv_lib)

target_include_directories(
  ${LIB_NAME} PUBLIC ${mogasens_csv_SOURCE_DIR}/compare_segmentation/include)

# APPLICATION
set(APP_NAME compare_segmentation_app)

add_executable(${APP_NAME} src/main.cpp)

target_link_libraries(${APP_NAME} PRIVATE ${LIB_NAME})

add_subdirectory(test)
