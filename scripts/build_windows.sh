#!/bin/bash

#parse parameter
if [ $1 ]
then
    OUTPUT="greenfoot++-$1"  
else 
    OUTPUT="main"
fi

#compiling glfw
cd "dependencies/glfw"
mkdir -p "x64"
cd "x64"
cmake cmake -G "Unix Makefiles" -DCMAKE_VERBOSE_MAKEFILE=ON -DBUILD_SHARED_LIBS=ON  ..
cmake --build .
cd "../../.."

#copy results
mkdir -p "lib"
mkdir -p "build/$1" 
mkdir -p "deploy"
cp "dependencies/glfw/x64/src/glfw3.dll" "lib"
cp "dependencies/glfw/x64/src/glfw3.dll" "build/$1"
cp "dependencies/glfw/x64/src/glfw3.dll" "deploy"
mv "deploy/glfw3.dll" "deploy/$1-glfw3.dll"

#build actual project
cd "build/$1"
cmake -G "Unix Makefiles" -DOUTPUTFILE=$OUTPUT ..
make -j2
cd ".."
cp "build/$1/$OUTPUT.exe" "deploy"