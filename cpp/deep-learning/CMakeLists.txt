cmake_minimum_required(VERSION 3.16)
project(MyWheelsDeepLearning VERSION 0.1.0.2 LANGUAGES CXX)

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

add_executable(deeplearning
  src/Main.cpp
  src/SimplePerceptron.cpp
  src/Function.cpp
)

target_include_directories(deeplearning PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(mywheelsmath STATIC IMPORTED)

set(MY_WHEELS_MATH_DIR "${CMAKE_SOURCE_DIR}/../math")
set(MY_WHEELS_MATH_NAME "mywheelsmath.a")

set_target_properties(mywheelsmath PROPERTIES
  IMPORTED_LOCATION_DEBUG "${MY_WHEELS_MATH_DIR}/debug/${MY_WHEELS_MATH_NAME}"
  IMPORTED_LOCATION_RELEASE "${MY_WHEELS_MATH_DIR}/bin/${MY_WHEELS_MATH_NAME}"
  INTERFACE_INCLUDE_DIRECTORIES "${MY_WHEELS_MATH_DIR}/include"
)

target_link_libraries(deeplearning PRIVATE mywheelsmath)

target_compile_options(deeplearning PRIVATE
  $<$<CONFIG:Debug>: -Wall -Wextra -pedantic -O0 -g>
  $<$<CONFIG:Release>: -O3 -march=native>
)