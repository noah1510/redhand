#!/bin/bash

#parse parameter
if [ $1 ]
then
    BUILDNAME="$1"
else 
    BUILDNAME="main"
fi

GLFWLIBNAME="libglfw.so"
REPOROOT="$(pwd)"
PROJECTNAME="greenfoot++"

#compiling glfw
cd "dependencies/glfw"
mkdir -p "x64"
cd "x64"
cmake -DBUILD_SHARED_LIBS=ON ..
cmake --build .
cd "../../.."

#compiling portaudio
cd "dependencies/portaudio"
mkdir -p "bin"
cd "bin"
cmake ".." -DCMAKE_INSTALL_PREFIX="../../.."
make -j2
make install
cd "../../.."

#copy results
mkdir -p "lib"
mkdir -p "build"
mkdir -p "build/$BUILDNAME"
mkdir -p "deploy"
cp "dependencies/glfw/x64/src/$GLFWLIBNAME" "lib/$GLFWLIBNAME"
cp "dependencies/glfw/x64/src/$GLFWLIBNAME" "lib/$GLFWLIBNAME.3"
cp "dependencies/glfw/x64/src/$GLFWLIBNAME" "build/$BUILDNAME"
cp "dependencies/glfw/x64/src/$GLFWLIBNAME" "deploy/$GLFWLIBNAME.3"
mv "deploy/$GLFWLIBNAME.3" "deploy/$BUILDNAME-$GLFWLIBNAME.3"

#build actual project
cd "build/$BUILDNAME"
cmake -G "Unix Makefiles" -DOUTPUTFILE="$PROJECTNAME-$BUILDNAME" -DREPOROOT=$REPOROOT "../.."
make -j2
cd "../.."
cp -r "build/$BUILDNAME/$PROJECTNAME-$BUILDNAME" "deploy"

LD_LIBRARY_PATH="$REPOROOT/lib"
export LD_LIBRARY_PATH