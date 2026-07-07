#!/bin/bash

mkdir -p build/clang
cmake -S . -B build/clang -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
cmake --build build/clang
./bin/mctools_status_ping
# ./bin/mctools_query