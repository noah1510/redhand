#!/bin/bash

mingw-get install msys-make

cd "dependencies/glfw"
mkdir -p "out"
cd "out"
cmake -DCMAKE_VERBOSE_MAKEFILE=ON -DBUILD_SHARED_LIBS=ON ..
cmake --build .
cd "../../.."
mkdir -p "lib"
cp "dependencies/glfw/out/src/libglfw.so" "lib"

mkdir -p "build" 
cd "build"

cmake ..
make -j2