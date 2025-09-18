cmake_minimum_required(VERSION 3.16)
project(MyWheelsTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_compile_options(
  -Wall
  -Wextra
  -O3
  -march=native
  -pedantic
  -fdiagnostics-color=always
)

add_executable(test Main.cpp)

add_library(math STATIC IMPORTED)

set_target_properties(math PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../lib/libmath.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../lib/include"
)

target_link_libraries(test PRIVATE math)