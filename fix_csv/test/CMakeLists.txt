include(GoogleTest)

set(TEST_NAME fmc_test)

set(TEST_SOURCES main.cpp remove_zeros_from_field_test.cpp)

add_executable(${TEST_NAME} ${TEST_SOURCES})

target_link_libraries(${TEST_NAME} PRIVATE fix_mogasens_csv_lib gtest_main)

gtest_discover_tests(${TEST_NAME} WORKING_DIRECTORY ${mogasens_csv_SOURCE_DIR})
