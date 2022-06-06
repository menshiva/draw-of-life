cmake_minimum_required(VERSION 3.22)
project(DrawOfLife C)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE MinSizeRel)
endif ()

if (MSVC)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /DEBUG")
else ()
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -ggdb -O0")
endif ()

set(CMAKE_C_STANDARD 11)

# sdl2
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
find_package(SDL2 REQUIRED COMPONENTS main)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

# src
file(GLOB_RECURSE SOURCES src/*)
add_executable(DrawOfLife ${SOURCES})

target_link_libraries(DrawOfLife ${SDL2_LIBS})
if (NOT WIN32)
    target_link_libraries(DrawOfLife m)
endif ()

# copy sdl2 dlls to output folder on windows
if (WIN32)
    foreach (DLL ${SDL2_DLLS})
        add_custom_command(TARGET DrawOfLife POST_BUILD COMMAND
                ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:DrawOfLife>)
    endforeach ()
endif ()
