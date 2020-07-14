# OBJECT LIBRARY
set(LIB_NAME feature_extraction_lib)

set(LIB_HEADERS include/f.hpp)

set(LIB_SOURCES src/f.cpp src/main.cpp)

add_library(${LIB_NAME} OBJECT ${LIB_HEADERS} ${LIB_SOURCES})

target_link_libraries(${LIB_NAME} PUBLIC Threads::Threads fmt::fmt scn::scn)

target_include_directories(
  ${LIB_NAME}
  PUBLIC ${mogasens_csv_SOURCE_DIR}/feature_extraction/include
         ${mogasens_csv_SOURCE_DIR}/external/philslib/include
         ${mogasens_csv_SOURCE_DIR}/external/csv-parser/single_include
         ${mogasens_csv_SOURCE_DIR}/external/tl/include
         ${mogasens_csv_SOURCE_DIR}/external/expected/include
         ${mogasens_csv_SOURCE_DIR}/external/optional/include)

# APPLICATION
set(APP_NAME feature_extraction_app)

add_executable(${APP_NAME} src/main.cpp)

target_link_libraries(${APP_NAME} PRIVATE ${LIB_NAME})

add_subdirectory(test)
