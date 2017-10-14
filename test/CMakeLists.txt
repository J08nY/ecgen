cmake_minimum_required(VERSION 2.8.11)

# This is an issue because criterion contains a header "io/output.h" which is
# after ecgen's "io/output.h" and so gets shadowed in the include directories
# list which breaks all kinds of things when criterion souces #include "io/output.h".
#
# add_subdirectory(lib/criterion)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/criterion/include)

find_library(criterion NAMES criterion PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib/criterion/build)

file(GLOB TEST_SRC "src/math/*.c" "src/gen/*.c" "src/cm/*.c" "src/invalid/*.c" "src/io/*.c" "src/exhaustive/*.c" "src/util/*.c")
file(GLOB TESTING_SRC "src/test/*.c")
add_executable(test_ecgen ${TEST_SRC} ${TESTING_SRC} ${SRC})

target_link_libraries(test_ecgen pthread rt pari parson sha1 ${criterion})

enable_testing()
add_test(NAME test_ecgen COMMAND test_ecgen)