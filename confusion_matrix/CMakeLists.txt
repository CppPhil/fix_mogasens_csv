# STATIC LIBRARY
set(LIB_NAME confusion_matrix_lib)

set(LIB_HEADERS include/manual_segmentation_point.hpp)

set(LIB_SOURCES src/manual_segmentation_point.cpp)

add_library(${LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})

target_link_libraries(${LIB_NAME} PUBLIC csv_lib)

target_include_directories(
  ${LIB_NAME} PUBLIC ${mogasens_csv_SOURCE_DIR}/confusion_matrix/include)

# APPLICATION
set(APP_NAME confusion_matrix_app)

add_executable(${APP_NAME} src/main.cpp)

target_link_libraries(${APP_NAME} PRIVATE ${LIB_NAME})

add_subdirectory(test)
