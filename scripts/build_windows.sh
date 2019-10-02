#!/bin/bash

#parse parameter
if [ $1 ]
then
    OUTPUT="greenfoot++-$1"
    BUILDDIR="build/$1"
    UP="../.."
else 
    OUTPUT="main"
    BUILDDIR="build"
    UP=".."
fi

LIBNAME="glfw3.dll"
REPOROOT="$(pwd)"

#compiling glfw
cd "dependencies/glfw"
mkdir -p "x64"
cd "x64"
cmake cmake -G "Unix Makefiles" -DCMAKE_VERBOSE_MAKEFILE=ON -DBUILD_SHARED_LIBS=ON  ..
cmake --build .
cd "../../.."

#copy results
mkdir -p "lib"
mkdir -p "build"
mkdir -p "$BUILDDIR"
mkdir -p "deploy"
cp "dependencies/glfw/x64/src/$LIBNAME" "lib"
cp "dependencies/glfw/x64/src/$LIBNAME" "$BUILDDIR"
cp "dependencies/glfw/x64/src/$LIBNAME" "deploy"
mv "deploy/$LIBNAME" "deploy/$1-$LIBNAME"

#build actual project
cd "$BUILDDIR"
cmake -G "Unix Makefiles" -DOUTPUTFILE=$OUTPUT -DREPOROOT=$REPOROOT $UP
make -j2
cd "$UP"
cp "$BUILDDIR/$OUTPUT.exe" "deploy"