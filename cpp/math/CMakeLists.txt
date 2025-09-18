cmake_minimum_required(VERSION 3.16)
project(MyWheelsMath VERSION 0.1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/../lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

add_compile_options(
  -Wall
  -Wextra
  -O3
  -march=native
  -pedantic
  -fdiagnostics-color=always
)

add_library(math STATIC 
  src/Function.cpp
)

target_include_directories(math PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_custom_command(TARGET math POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_DIR}/include"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/include" "${OUTPUT_DIR}/include"
)