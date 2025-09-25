cmake_minimum_required(VERSION 3.16)
project(MyWheelsDeepLearning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin")
if (NOT MSVC)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OUTPUT_DIR "${OUTPUT_DIR}/Debug")
  elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(OUTPUT_DIR "${OUTPUT_DIR}/Release")
  endif ()
endif ()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

add_executable(deep_learning src/Main.cpp)

add_library(math STATIC IMPORTED)

set(MATH_LIB_DIR "${CMAKE_SOURCE_DIR}/../math")
if (MSVC)
  set_target_properties(math PROPERTIES
    IMPORTED_LOCATION_DEBUG "${MATH_LIB_DIR}/bin/Debug/math.lib"
    IMPORTED_LOCATION_RELEASE "${MATH_LIB_DIR}/bin/Release/math.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${MATH_LIB_DIR}/include"
  )
else ()
  set_target_properties(math PROPERTIES
    IMPORTED_LOCATION_DEBUG "${MATH_LIB_DIR}/bin/Debug/libmath.a"
    IMPORTED_LOCATION_RELEASE "${MATH_LIB_DIR}/bin/Release/libmath.a"
    INTERFACE_INCLUDE_DIRECTORIES "${MATH_LIB_DIR}/include"
  )
endif ()

target_link_libraries(deep_learning PRIVATE math)

target_compile_options(deep_learning PRIVATE
  # Clang
  $<$<CXX_COMPILER_ID:Clang>: -Wall -Wextra -pedantic -march=native>
  # Clang, Release
  $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CONFIG:Release>>: -O3>
  # Clang, Debug
  $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CONFIG:Debug>>:-O0 -g>
  # MSVC
  $<$<CXX_COMPILER_ID:MSVC>: /W4 /permissive- /EHsc>
  # MSVC, Release
  $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>: /O2 /GL /LTCG /OPT:REF /OPT:ICF /arch:AVX2>
  # MSVC, Debug
  $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>: /Od /RTC1 /Zi /DEBUG>
)