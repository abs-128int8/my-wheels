cmake_minimum_required(VERSION 3.16)
project(MyWheelsMath VERSION 0.1.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/debug")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
  set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
endif ()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

add_library(mywheelsmath STATIC
  src/BigInt.cpp
)

set_target_properties(mywheelsmath PROPERTIES
  PREFIX "")

target_include_directories(mywheelsmath PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(mywheelsmath PRIVATE
  $<$<CONFIG:Debug>: -Wall -Wextra -pedantic -O0 -g>
  $<$<CONFIG:Release>: -O3>
)