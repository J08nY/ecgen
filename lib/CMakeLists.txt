cmake_minimum_required(VERSION 3.1O)
project(ecgen_libs C)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -g -Wall -Werror -pedantic")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG -O3 -Wall")

file(GLOB PARSON_SRC "parson/*.c")
file(GLOB SHA1_SRC "sha1/*.c")

add_library(parson STATIC ${PARSON_SRC})
add_library(sha1 STATIC ${SHA1_SRC})
