#!/bin/bash

#parse parameter
if [ $1 == "" ]
then
    OUTPUT="main.out"
else 
    OUTPUT="greenfoot++-$1"
fi

#compiling glfw
cd "dependencies/glfw"
mkdir -p "out"
cd "out"
cmake -DBUILD_SHARED_LIBS=ON ..
cmake --build .
cd "../../.."

#copy results
mkdir -p "lib"
mkdir -p "build"
mkdir -p "deploy"
cp "dependencies/glfw/out/src/libglfw.so" "lib"
cp "dependencies/glfw/out/src/libglfw.so" "build"
cp "dependencies/glfw/out/src/libglfw.so" "deploy"
mv "deploy/libglfw.so" "deploy/$1-libglfw.so"

#build actual project
cd "build"
cmake -G "Unix Makefiles" -DOUTPUTFILE=$OUTPUT ..
make -j2
cd ".."

#clean up
cp "build/$OUTPUT" "deploy"
rm -r build