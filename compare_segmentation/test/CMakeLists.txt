include(GoogleTest)

set(TEST_NAME compare_segmentation_test)

set(TEST_SOURCES main.cpp log_files_test.cpp)

add_executable(${TEST_NAME} ${TEST_SOURCES})

target_link_libraries(${TEST_NAME} PRIVATE compare_segmentation_lib gtest_main)

gtest_discover_tests(${TEST_NAME} WORKING_DIRECTORY ${mogasens_csv_SOURCE_DIR})
