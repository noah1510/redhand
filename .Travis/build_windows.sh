#!/bin/bash

cd "dependencies/doxygen"
mkdir "build"
cd "build"
cmake -G "MinGW Makefiles" ..
make
make install
cd "../.."

cd "include/glfw"
cmake -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=ON .
make -j2
cd "../.."

mkdir -p "build" 
cd "build"

cmake ..
make -j2