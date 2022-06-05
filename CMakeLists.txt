cmake_minimum_required(VERSION 3.22)
project(DrawOfLife C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE MinSizeRel)
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -ggdb -O0")
set(CMAKE_C_STANDARD 11)

add_executable(DrawOfLife src/main.c)

find_package(SDL2 REQUIRED)
target_link_libraries(DrawOfLife ${SDL2_LIBRARIES} m)
