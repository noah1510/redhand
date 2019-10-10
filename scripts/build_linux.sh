#!/bin/bash

#parse parameter
if [ $1 ]
then
    BUILDNAME="$1"
else 
    BUILDNAME="main"
fi

LIBNAME="libglfw.so"
REPOROOT="$(pwd)"
PROJECTNAME="greenfoot++"

#compiling glfw
cd "dependencies/glfw"
mkdir -p "x64"
cd "x64"
cmake -DBUILD_SHARED_LIBS=ON ..
cmake --build .
cd "../../.."

#copy results
mkdir -p "lib"
mkdir -p "build"
mkdir -p "build/$BUILDNAME"
mkdir -p "deploy"
cp "dependencies/glfw/x64/src/$LIBNAME" "lib/$LIBNAME"
cp "dependencies/glfw/x64/src/$LIBNAME" "lib/$LIBNAME.3"
cp "dependencies/glfw/x64/src/$LIBNAME" "build/$BUILDNAME"
cp "dependencies/glfw/x64/src/$LIBNAME" "deploy/$LIBNAME.3"
mv "deploy/$LIBNAME.3" "deploy/$BUILDNAME-$LIBNAME.3"

#build actual project
cd "build/$BUILDNAME"
cmake -G "Unix Makefiles" -DOUTPUTFILE="$PROJECTNAME-$BUILDNAME" -DREPOROOT=$REPOROOT "../.."
make -j2
cd "../.."
cp -r "build/$BUILDNAME/$PROJECTNAME-$BUILDNAME" "deploy"

LD_LIBRARY_PATH="$REPOROOT/lib"
export LD_LIBRARY_PATH