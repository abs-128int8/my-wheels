cmake_minimum_required(VERSION 3.16)
project(MyWheelsMcTools VERSION 0.1.0.0 LANGUAGES CXX)

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

add_library(mywheelsmctools STATIC
  src/SocketClient.cpp
  src/PacketWriter.cpp
  src/PacketReader.cpp)

set_target_properties(mywheelsmctools PROPERTIES
  PREFIX "")

target_include_directories(mywheelsmctools PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(mywheelsmctools PRIVATE
  $<$<CONFIG:Debug>: -Wall -Wextra -pedantic -O0 -g>
  $<$<CONFIG:Release>: -O3 -march=native>
)

add_executable(mctools_status_ping
  samples/StatusPing.cpp)

target_link_libraries(mctools_status_ping PRIVATE mywheelsmctools)

target_compile_options(mctools_status_ping PRIVATE
  $<$<CONFIG:Debug>: -Wall -Wextra -pedantic -O0 -g>
  $<$<CONFIG:Release>: -O3 -march=native>
)

add_executable(mctools_query
  samples/Query.cpp)

target_link_libraries(mctools_query PRIVATE mywheelsmctools)

target_compile_options(mctools_query PRIVATE
  $<$<CONFIG:Debug>: -Wall -Wextra -pedantic -O0 -g>
  $<$<CONFIG:Release>: -O3 -march=native>
)