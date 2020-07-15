include(GoogleTest)

set(TEST_NAME csv_lib_test)

set(TEST_SOURCES
    channel_test.cpp
    column_test.cpp
    data_point_test.cpp
    error_test.cpp
    exception_test.cpp
    main.cpp
    sensor_test.cpp
    to_string_test.cpp
    read_csv_file_test.cpp
    data_set_test.cpp)

add_executable(${TEST_NAME} ${TEST_SOURCES})

target_include_directories(
  ${TEST_NAME} PRIVATE ${mogasens_csv_SOURCE_DIR}/csv_lib/include/cl)

target_link_libraries(${TEST_NAME} PRIVATE csv_lib gtest_main)

gtest_discover_tests(${TEST_NAME} WORKING_DIRECTORY ${mogasens_csv_SOURCE_DIR})
