# STATIC LIBRARY
set(LIB_NAME confusion_matrix_lib)

set(LIB_HEADERS
    include/closest_one.hpp
    include/configuration.hpp
    include/confusion_matrix.hpp
    include/create_segmentation_results.hpp
    include/csv_file_info.hpp
    include/data_set_identifier.hpp
    include/distance.hpp
    include/distance_score.hpp
    include/fetch.hpp
    include/imu.hpp
    include/interpolated_data_set_paths.hpp
    include/manual_segmentation_point.hpp
    include/order_configurations_by_quality.hpp
    include/python_output.hpp
    include/segment.hpp
    include/split_string.hpp)

set(LIB_SOURCES
    src/closest_one.cpp
    src/configuration.cpp
    src/confusion_matrix.cpp
    src/create_segmentation_results.cpp
    src/csv_file_info.cpp
    src/data_set_identifier.cpp
    src/distance.cpp
    src/distance_score.cpp
    src/imu.cpp
    src/interpolated_data_set_paths.cpp
    src/manual_segmentation_point.cpp
    src/order_configurations_by_quality.cpp
    src/python_output.cpp
    src/segment.cpp
    src/split_string.cpp)

add_library(${LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})

target_link_libraries(${LIB_NAME} PUBLIC csv_lib)

target_include_directories(
  ${LIB_NAME} PUBLIC ${mogasens_csv_SOURCE_DIR}/confusion_matrix/include)

# APPLICATION
set(APP_NAME confusion_matrix_app)

add_executable(${APP_NAME} src/main.cpp)

target_link_libraries(${APP_NAME} PRIVATE ${LIB_NAME})

add_subdirectory(test)
