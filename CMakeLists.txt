cmake_minimum_required(VERSION 2.8.11)
project(ecgen C)

set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib)
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -g -Wall -Werror -pedantic")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG -O3 -Wall")

include_directories(src)
include_directories(lib)

find_library(parson parson/libparson.a)
find_library(sha1 sha1/libsha1.a)

file(GLOB SRC "src/math/*.c" "src/cm/*.c" "src/invalid/*.c" "src/io/*.c" "src/exhaustive/*.c" "src/util/*.c")
set(ECGEN_SRC "src/ecgen.c" ${SRC})
set(ECONVERT_SRC "src/econvert.c")

add_executable(ecgen ${ECGEN_SRC})
add_executable(econvert ${ECONVERT_SRC})

target_link_libraries(ecgen pthread rt pari ${parson} ${sha1})
target_link_libraries(econvert pthread rt pari ${parson} ${sha1})