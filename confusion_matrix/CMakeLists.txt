# STATIC LIBRARY
set(LIB_NAME confusion_matrix_lib)

set(LIB_HEADERS
    include/data_set_identifier.hpp include/interpolated_data_set_paths.hpp
    include/manual_segmentation_point.hpp include/segment.hpp
    include/split_string.hpp)

set(LIB_SOURCES
    src/data_set_identifier.cpp src/interpolated_data_set_paths.cpp
    src/manual_segmentation_point.cpp src/segment.cpp src/split_string.cpp)

add_library(${LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})

target_link_libraries(${LIB_NAME} PUBLIC csv_lib)

target_include_directories(
  ${LIB_NAME} PUBLIC ${mogasens_csv_SOURCE_DIR}/confusion_matrix/include)

# APPLICATION
set(APP_NAME confusion_matrix_app)

add_executable(${APP_NAME} src/main.cpp)

target_link_libraries(${APP_NAME} PRIVATE ${LIB_NAME})

add_subdirectory(test)
