cmake_minimum_required(VERSION 2.8.11)
project(test_ecgen C)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/criterion/include)

message("${CMAKE_LIBRARY_PATH}")

find_library(criterion NAMES criterion PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib/criterion/build)

file(GLOB TEST_SRC "src/math/*.c" "src/gen/*.c" "src/cm/*.c" "src/invalid/*.c" "src/io/*.c" "src/exhaustive/*.c" "src/util/*.c")
add_executable(test_ecgen ${TEST_SRC} ${SRC})

target_link_libraries(test_ecgen pthread rt pari ${parson} ${sha1} ${criterion})

enable_testing()
add_test(NAME test_ecgen COMMAND test_ecgen)