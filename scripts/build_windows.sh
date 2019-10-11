#!/bin/bash

#parse parameter
if [ $1 ]
then
    BUILDNAME="$1"
else 
    BUILDNAME="main"
fi

LIBNAME="glfw3.dll"
REPOROOT="$(pwd)"
PROJECTNAME="greenfoot++"

#compiling glfw
cd "dependencies/glfw"
mkdir -p "x64"
cd "x64"
cmake -G "Unix Makefiles" -DCMAKE_VERBOSE_MAKEFILE=ON -DBUILD_SHARED_LIBS=ON  ..
cmake --build .
cd "../../.."

#compiling portaudio
cd "dependencies/portaudio"
mkdir -p "bin"
cd "bin"
cmake ".." -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="../../.."
make -j2
make install
cd "../../.."

#copy results
mkdir -p "lib"
mkdir -p "build"
mkdir -p "build/$BUILDNAME"
mkdir -p "deploy"
cp "dependencies/glfw/x64/src/$LIBNAME" "lib"
cp "dependencies/glfw/x64/src/$LIBNAME" "$BUILDDIR"
cp "dependencies/glfw/x64/src/$LIBNAME" "deploy"
mv "deploy/$LIBNAME" "deploy/$BUILDNAME-$LIBNAME"

#build actual project
cd "build/$BUILDNAME"
cmake -G "Unix Makefiles" -DOUTPUTFILE="$PROJECTNAME-$BUILDNAME" -DREPOROOT=$REPOROOT "../.."
make -j2
cd "../.."
cp -r "build/$BUILDNAME/$PROJECTNAME-$BUILDNAME.exe" "deploy"

LD_LIBRARY_PATH="$REPOROOT/lib"
export LD_LIBRARY_PATH
