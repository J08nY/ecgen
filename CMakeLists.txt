cmake_minimum_required(VERSION 2.8.11)
project(ecgen)

set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib)

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -Wall")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -Wall")

add_custom_command(
        OUTPUT gp.c gp.h
        COMMAND gp2c -g -i4 gp/gp.gp | clang-format | awk "BEGIN{print \"#ifndef GP_H\" > \"gp.h\"; print \"#define GP_H\" >> \"gp.h\";} { if\(found\) print >> \"gp.c\"; else print >> \"gp.h\"} /End of prototype/{found=1; print \"\#include \\\"gp.h\\\"\" > \"gp.c\"; print \"#endif //GP_H\" >> \"gp.h\"}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src"
        COMMENT "gp2c gp.gp"
        VERBATIM
)

add_custom_target(gp2c ALL DEPENDS gp.c gp.h)

include_directories(src)
include_directories(lib)

file(GLOB SOURCES "src/*.c" "src/*.h")

add_executable(ecgen ${SOURCES})

target_link_libraries(ecgen pari)
find_library(parson parson/libparson.a)
target_link_libraries(ecgen ${parson})