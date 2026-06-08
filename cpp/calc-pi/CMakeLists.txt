cmake_minimum_required(VERSION 3.16)
project(MyWheelsCalcPi VERSION 0.1.0.0 LANGUAGES CXX)

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

add_executable(calcpi
  src/main.cpp)

target_include_directories(calcpi PRIVATE include)

target_compile_options(calcpi PRIVATE
  $<$<CONFIG:Debug>: -Wall -Wextra -pedantic -O0 -g>
  $<$<CONFIG:Release>: -O3 -march=native>
)