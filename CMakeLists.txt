cmake_minimum_required(VERSION 3.10)
project(ecgen C)

add_subdirectory(lib)

file(GLOB SRC "src/math/*.c" "src/obj/*.c" "src/gen/*.c" "src/cm/*.c" "src/invalid/*.c" "src/io/*.c" "src/exhaustive/*.c" "src/misc/*.c" "src/util/*.c")
set(ECGEN_SRC "src/ecgen.c" ${SRC})

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -g -Wall -Werror -flto")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG -O3 -Wall -flto")

include_directories(src)
include_directories(lib)

#add_subdirectory(test)

add_executable(ecgen ${ECGEN_SRC})

target_link_libraries(ecgen pthread rt pari parson sha1)


