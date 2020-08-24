#!/bin/bash

if [ "$(basename $(pwd))" == "scripts" ]
then
    cd ..
fi

#clean up
rm -rf "build"
rm -rf "CMakeFiles"
rm -rf "cmake_install.cmake"
rm -rf "CMakeCache.txt"
rm -rf "lib"
rm -rf "deploy"
rm -rf "doc/html"
rm -rf "doc/xml"
rm -rf ".cache"
rm -rf "doc/def"
rm -rf "doc/man"
rm -rf "obj-x86_64-linux-gnu"
rm -rf "include/redhand/glad"
rm -rf "include/redhand/KHR"

mkdir "lib"
mkdir "deploy"
