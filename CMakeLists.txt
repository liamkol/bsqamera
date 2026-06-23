cmake_minimum_required(VERSION 3.22)
project(qamera)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED ANDROID_ABI)
    set(ANDROID_ABI arm64-v8a)
endif()

add_compile_options(
    -fno-rtti
    -funwind-tables
    -fexceptions
    -O2
    -Wall
)

add_compile_definitions(
    VERSION="0.1.0"
    ID="qamera"
)

include(${CMAKE_CURRENT_SOURCE_DIR}/qpm_defines.cmake)

file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(qamera SHARED ${SOURCE_FILES})

target_include_directories(qamera PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include(${CMAKE_CURRENT_SOURCE_DIR}/extern.cmake)

target_link_libraries(qamera PRIVATE log)

set_target_properties(qamera PROPERTIES
    OUTPUT_NAME "qamera"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)
