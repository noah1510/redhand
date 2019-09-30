#!/bin/bash

cd "dependencies/glfw"
cmake -DBUILD_SHARED_LIBS=ON .
make -j2
cd "../.."

mkdir -p "build"
cd "build"

cmake ..
make -j2