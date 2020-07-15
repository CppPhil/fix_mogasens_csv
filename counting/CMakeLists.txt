# STATIC LIBRARY
set(LIB_NAME counting_lib)

set(LIB_HEADERS
    include/above_threshold.hpp include/average_comparison_value_calculator.hpp
    include/is_relevant.hpp include/percentage_of.hpp
    include/run_above_threshold.hpp)

set(LIB_SOURCES
    src/above_threshold.cpp src/average_comparison_value_calculator.cpp
    src/main.cpp src/run_above_threshold.cpp)

add_library(${LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})

target_link_libraries(${LIB_NAME} PUBLIC Threads::Threads csv_lib)

target_include_directories(${LIB_NAME}
                           PUBLIC ${mogasens_csv_SOURCE_DIR}/counting/include)

# APPLICATION
set(APP_NAME counting_app)

add_executable(${APP_NAME} src/main.cpp)

target_link_libraries(${APP_NAME} PRIVATE ${LIB_NAME})

add_subdirectory(test)
