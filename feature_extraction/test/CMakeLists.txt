include(GoogleTest)

set(TEST_NAME feature_extraction_test)

set(TEST_SOURCES f_test.cpp main.cpp)

add_executable(${TEST_NAME} ${TEST_SOURCES})

target_link_libraries(${TEST_NAME} PRIVATE feature_extraction_lib gtest_main)

gtest_discover_tests(${TEST_NAME} WORKING_DIRECTORY ${mogasens_csv_SOURCE_DIR})
