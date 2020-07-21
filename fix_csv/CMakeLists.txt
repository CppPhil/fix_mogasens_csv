# STATIC LIBRARY
set(LIB_NAME fix_mogasens_csv_lib)

set(LIB_HEADERS
    include/remove_zeros_from_field.hpp include/columns.hpp
    include/delete_non_bosch_sensors.hpp include/adjust_hardware_timestamp.hpp)

set(LIB_SOURCES
    src/remove_zeros_from_field.cpp src/delete_non_bosch_sensors.cpp
    src/adjust_hardware_timestamp.cpp)

add_library(${LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})

target_link_libraries(${LIB_NAME} PUBLIC csv_lib)

target_include_directories(${LIB_NAME}
                           PUBLIC ${mogasens_csv_SOURCE_DIR}/fix_csv/include)

# APPLICATION
set(APP_NAME fix_mogasens_csv_app)

add_executable(${APP_NAME} src/main.cpp)

target_link_libraries(${APP_NAME} PRIVATE ${LIB_NAME})

add_subdirectory(test)
