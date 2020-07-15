include(GoogleTest)

set(TEST_NAME counting_test)

set(TEST_SOURCES above_threshold_test.cpp main.cpp percentage_of_test.cpp)

add_executable(${TEST_NAME} ${TEST_SOURCES})

target_link_libraries(${TEST_NAME} PRIVATE counting_lib gtest_main)

gtest_discover_tests(${TEST_NAME} WORKING_DIRECTORY ${mogasens_csv_SOURCE_DIR})
