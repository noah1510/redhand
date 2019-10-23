#!/bin/bash

#parse parameter
if [ $1 ]
then
    BUILDNAME="$1"
else 
    BUILDNAME="main"
fi

REPOROOT="$(pwd)"
PROJECTNAME="greenfoot++"

if [ "$OSTYPE" == "linux-gnu" ]
then
    # Linux
    GLFWLIBNAME="libglfw.so"
    SFMLAUDIOLIB="libsfml-audio.so" 
    SFMLGRAPICSLIB="libsfml-graphics.so" 
    SFMLWINDOWLIB="libsfml-window.so"
    SFMLSYSTEMLIB="libsfml-system.so"

    EXECUTABLE="$PROJECTNAME-$BUILDNAME"

elif [ "$OSTYPE" == "darwin"* ]
then
    # Mac OSX
    GLFWLIBNAME="libglfw.so"
    SFMLAUDIOLIB="libsfml-audio.so" 
    SFMLGRAPICSLIB="libsfml-graphics.so" 
    SFMLWINDOWLIB="libsfml-window.so"
    SFMLSYSTEMLIB="libsfml-system.so"

    EXECUTABLE="$PROJECTNAME-$BUILDNAME"
        
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]
then
    # Lightweight shell and GNU utilities compiled for Windows (part of MinGW)
    # or 
    # POSIX compatibility layer and Linux environment emulation for Windows
    GLFWLIB="glfw3.dll"
    SFMLAUDIOLIB="sfml-audio-2.dll" 
    SFMLGRAPICSLIB="sfml-graphics-2.dll" 
    SFMLWINDOWLIB="sfml-window-2.dll"
    SFMLSYSTEMLIB="sfml-system-2.dll"

    EXECUTABLE="$PROJECTNAME-$BUILDNAME.exe"

    alias make='mingw32-make'

else
        # Unknown os
        exit 1
fi

#compiling glfw
cd "dependencies/glfw"
mkdir -p "x64"
cd "x64"
cmake -G "Unix Makefiles" -DBUILD_SHARED_LIBS=ON  ..
cmake --build .
cd "../../.."

#compiling SFML
cd "dependencies/SFML"
mkdir -p "bin"
cd "bin"
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="Release" -DBUILD_SHARED_LIBS=ON ..
make -j2
cd "../../.."

#copy results
mkdir -p "lib"
mkdir -p "build"
mkdir -p "build/$BUILDNAME"
mkdir -p "deploy"

cp "dependencies/glfw/x64/src/$GLFWLIBNAME" "lib"
cp "dependencies/SFML/bin/lib/$SFMLAUDIOLIB" "lib"
cp "dependencies/SFML/bin/lib/$SFMLGRAPICSLIB" "lib"
cp "dependencies/SFML/bin/lib/$SFMLWINDOWLIB" "lib"
cp "dependencies/SFML/bin/lib/$SFMLSYSTEMLIB" "lib"

cp "dependencies/glfw/x64/src/$GLFWLIBNAME" "build/$BUILDNAME"
cp "dependencies/SFML/bin/lib/$SFMLAUDIOLIB" "build/$BUILDNAME"
cp "dependencies/SFML/bin/lib/$SFMLGRAPICSLIB" "build/$BUILDNAME"
cp "dependencies/SFML/bin/lib/$SFMLWINDOWLIB" "build/$BUILDNAME"
cp "dependencies/SFML/bin/lib/$SFMLSYSTEMLIB" "build/$BUILDNAME"

cp "dependencies/glfw/x64/src/$GLFWLIBNAME" "deploy"
cp "dependencies/SFML/bin/lib/$SFMLAUDIOLIB" "deploy"
cp "dependencies/SFML/bin/lib/$SFMLGRAPICSLIB" "deploy"
cp "dependencies/SFML/bin/lib/$SFMLWINDOWLIB" "deploy"
cp "dependencies/SFML/bin/lib/$SFMLSYSTEMLIB" "deploy"

cp -r "dependencies/SFML/include/SFML" "include"

cp "deploy/$GLFWLIBNAME" "deploy/$BUILDNAME-$GLFWLIBNAME"
cp "deploy/$SFMLAUDIOLIB" "deploy/$BUILDNAME-$SFMLAUDIOLIB"
cp "deploy/$SFMLGRAPICSLIB" "deploy/$BUILDNAME-$SFMLGRAPICSLIB"
cp "deploy/$SFMLWINDOWLIB" "deploy/$BUILDNAME-$SFMLWINDOWLIB"
cp "deploy/$SFMLSYSTEMLIB" "deploy/$BUILDNAME-$SFMLSYSTEMLIB"

#build actual project
cd "build/$BUILDNAME"
cmake -G "Unix Makefiles" -DOUTPUTFILE="$PROJECTNAME-$BUILDNAME" -DREPOROOT=$REPOROOT "../.."
make -j2
cd "../.."
cp -r "build/$BUILDNAME/$EXECUTABLE" "deploy"

LD_LIBRARY_PATH="$REPOROOT/lib"
export LD_LIBRARY_PATH
