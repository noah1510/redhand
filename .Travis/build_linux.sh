#!/bin/bash

cd "dependencies/glfw"
mkdir -p "out"
cd "out"
cmake -DBUILD_SHARED_LIBS=ON ..
make -j2
cd "../../.."
mkdir -p "lib"
cp "dependencies/glfw/out/src/libglfw.so" "lib"

mkdir -p "build"
cd "build"

cmake ..
make -j2