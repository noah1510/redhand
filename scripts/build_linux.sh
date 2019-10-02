#!/bin/bash

#parse parameter
if [ $1 ]
then
    OUTPUT="greenfoot++-$1"
else 
    OUTPUT="main.out"
fi

#compiling glfw
cd "dependencies/glfw"
mkdir -p "x64"
cd "x64"
cmake -DBUILD_SHARED_LIBS=ON ..
cmake --build .
cd "../../.."

#copy results
mkdir -p "lib"
mkdir -p "build/$1"
mkdir -p "deploy"
cp "dependencies/glfw/x64/src/libglfw.so" "lib"
cp "dependencies/glfw/x64/src/libglfw.so" "build/$1"
cp "dependencies/glfw/x64/src/libglfw.so" "deploy"
mv "deploy/libglfw.so" "deploy/$1-libglfw.so"

#build actual project
cd "build/$1"
cmake -G "Unix Makefiles" -DOUTPUTFILE=$OUTPUT ..
make -j2
cd ".."
cp "build/$1/$OUTPUT" "deploy"