#!/bin/bash
git clone https://github.com/glfw/glfw.git include/glfw

cd "dependencies/doxygen"
mkdir "build"
cd "build"
cmake -G "Unix Makefiles" ..
make
make install
cd "../.."

cd "include/glfw"
cmake .
make -j2
cd "../.."

mkdir -p "build" 
cd "build"

cmake ..
make -j2