cmake_minimum_required(VERSION 2.8.11)
project(ecgen)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")

add_custom_command(
        OUTPUT gp.c gp.h
        COMMAND gp2c -g -i4 gp/gp.gp | clang-format | awk "BEGIN{print \"#ifndef GP_H\" > \"gp.h\"; print \"#define GP_H\" >> \"gp.h\";} { if\(found\) print >> \"gp.c\"; else print >> \"gp.h\"} /End of prototype/{found=1; print \"\#include \\\"gp.h\\\"\" > \"gp.c\"; print \"#endif //GP_H\" >> \"gp.h\"}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src"
        COMMENT "gp2c gp.gp"
        VERBATIM
)

add_custom_target(gp2c ALL DEPENDS gp.c gp.h)

include_directories(src)

file(GLOB SOURCES "src/*.c")

add_executable(ecgen ${SOURCES})
add_dependencies(ecgen gp2c)

target_link_libraries(ecgen pari)