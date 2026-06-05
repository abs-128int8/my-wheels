cmake_minimum_required(VERSION 3.16)
project(MyWheelsCalcPi VERSION 0.1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(calc_pi
  src/main.cpp)

target_include_directories(calc_pi PRIVATE include)

target_compile_options(calc_pi PRIVATE
  $<$<CONFIG:Debug>: -Wall -Wextra -pedantic -O0 -g>
  $<$<CONFIG:Release>: -O3 -march=native>
)