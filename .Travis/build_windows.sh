#!/bin/bash

cd "dependencies/glfw"
mkdir -p "out"
cd "out"
cmake -DCMAKE_VERBOSE_MAKEFILE=ON -DBUILD_SHARED_LIBS=ON ..
cmake --build .
echo "--- contentes of win32 ---"
ls -a "win32"
echo "--- contentes of win32/Debug ---"
ls -a "win32/Debug"
echo "--- contentes of Debug ---"
ls -a "Debug"
cd "../../.."
mkdir -p "lib"
cp "dependencies/glfw/out/src/win32/Debug/glfw3.dll" "lib"

mkdir -p "build" 
cd "build"

cmake ..
make -j2