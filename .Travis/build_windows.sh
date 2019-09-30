#!/bin/bash

cd "dependencies/glfw"
mkdir "out"
cd "out"
cmake -DBUILD_SHARED_LIBS=ON ..
make -j2
cd "../../.."
mkdir "lib"
cp "dependencies/glfw/out/src/libglfw.so" "lib"

mkdir -p "build" 
cd "build"

cmake ..
make -j2