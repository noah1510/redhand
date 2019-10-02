#!/bin/bash

#parse parameter
if [ $1 == "" ]
then
    OUTPUT="main"
else 
    OUTPUT="greenfoot++-$1"
fi
if [ $2 == "gitbash" ]
then
	wget https://netix.dl.sourceforge.net/project/ezwinports/make-4.2.1-with-guile-w32-bin.zip
	unzip -n make-4.2.1-with-guile-w32-bin.zip -d ../..
fi

#compiling glfw
cd "dependencies/glfw"
mkdir -p "out"
cd "out"
cmake cmake -G "Unix Makefiles" -DCMAKE_VERBOSE_MAKEFILE=ON -DBUILD_SHARED_LIBS=ON  ..
cmake --build .
cd "../../.."

#copy results
mkdir -p "lib"
mkdir -p "build" 
mkdir -p "deploy"
cp "dependencies/glfw/out/src/glfw3.dll" "lib"
cp "dependencies/glfw/out/src/glfw3.dll" "build"
cp "dependencies/glfw/out/src/glfw3.dll" "deploy"
mv "deploy/glfw3.dll" "deploy/$1-glfw3.dll"

#build actual project
cd "build"
cmake -G "Unix Makefiles" -DOUTPUTFILE=$OUTPUT ..
make -j2
cd ".."

#clean up
cp "build/$OUTPUT.exe" "deploy"
rm -r build
