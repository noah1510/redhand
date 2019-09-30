#!/bin/bash

git clone https://github.com/glfw/glfw.git include/glfw

cd "include/glfw"
cmake .
make -j2

cd "../.."
mkdir -p "build" 
cd "build"

cmake ..
make -j2