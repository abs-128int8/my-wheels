md build\msvc
cmake -S . -B build/msvc -G "Visual Studio 17 2022" -A "x64" -DCMAKE_CXX_COMPILER=cl
cmake --build build/msvc --config Debug
cmake --build build/msvc --config Release