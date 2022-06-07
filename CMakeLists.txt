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
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/libs/SDL2)
find_package(SDL2 REQUIRED COMPONENTS main)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

# sources
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/src/*.h)
add_executable(DrawOfLife ${SOURCES} ${HEADERS})

target_link_libraries(DrawOfLife PRIVATE ${SDL2_LIBS})
if (NOT MSVC)
    target_link_libraries(DrawOfLife PRIVATE m)
endif ()

# copy sdl2 dlls to output folder on windows
if (MSVC)
    foreach (DLL ${SDL2_DLLS})
        add_custom_command(TARGET DrawOfLife POST_BUILD COMMAND
                ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:DrawOfLife>)
    endforeach ()
endif ()
