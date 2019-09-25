#!/bin/bash
git clone https://github.com/nothings/stb.git include/stb

mkdir -p build 
cd build

cmake ..
make -j2