#!/bin/bash

git clone https://github.com/nothings/stb.git include/stb
git clone https://github.com/glfw/glfw.git include/glfw
git clone https://github.com/g-truc/glm include/gitglm

cd "include/glfw"
cmake -DBUILD_SHARED_LIBS=ON .
make -j2

cd "../.."
mkdir -p "build" 
cd "build"

cmake ..
make -j2