#!/bin/bash

mingw-get install msys-make

cd "dependencies/glfw"
mkdir -p "out"
cd "out"
cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..
cmake --build .
ls -a
cd "../../.."
mkdir -p "lib"
cp "dependencies/glfw/out/src/glfw3dll.lib" "lib"

mkdir -p "build" 
cd "build"

cmake ..
make -j2