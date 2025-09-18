#!/bin/bash

mkdir -p build
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++
cmake --build build