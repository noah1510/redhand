#!/bin/bash
git clone https://github.com/nothings/stb.git include/stb
git clone https://github.com/g-truc/glm include/gitglm

mkdir -p "build"
cd "build"

cmake ..
make -j2