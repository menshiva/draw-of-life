cmake_minimum_required(VERSION 3.22)
project(DrawOfLife C)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -ggdb -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
set(CMAKE_C_STANDARD 11)

# src files
file(GLOB_RECURSE src_files "src/*")

find_package(SDL2 REQUIRED)
include_directories(DrawOfLife ${SDL2_INCLUDE_DIRS})

add_executable(DrawOfLife ${src_files})
target_link_libraries(DrawOfLife ${SDL2_LIBRARIES})
