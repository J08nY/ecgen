cmake_minimum_required(VERSION 2.8.11)
project(ecgen C)

add_subdirectory(lib)

file(GLOB SRC "src/math/*.c" "src/gen/*.c" "src/cm/*.c" "src/invalid/*.c" "src/io/*.c" "src/exhaustive/*.c" "src/misc/*.c" "src/util/*.c")
set(ECGEN_SRC "src/ecgen.c" ${SRC})
set(ECONVERT_SRC "src/econvert.c")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -g -Wall -Werror -pedantic")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG -O3 -Wall")

include_directories(src)
include_directories(lib)

add_subdirectory(test)

add_executable(ecgen ${ECGEN_SRC})
add_executable(econvert ${ECONVERT_SRC})

target_link_libraries(ecgen pthread rt pari parson sha1)
target_link_libraries(econvert pthread rt pari parson sha1)


