include(GoogleTest)

set(TEST_NAME confusion_matrix_test)

set(TEST_SOURCES interpolated_data_set_paths_test.cpp main.cpp
                 manual_segmentation_point_test.cpp)

add_executable(${TEST_NAME} ${TEST_SOURCES})

target_link_libraries(${TEST_NAME} PRIVATE confusion_matrix_lib gtest_main)

gtest_discover_tests(${TEST_NAME} WORKING_DIRECTORY ${mogasens_csv_SOURCE_DIR})
