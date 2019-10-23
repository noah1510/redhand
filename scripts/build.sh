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
    echo "script running on linux"

    GLFWLIBNAME="libglfw.so"
    SFMLAUDIOLIB="libsfml-audio.so" 
    SFMLGRAPICSLIB="libsfml-graphics.so" 
    SFMLWINDOWLIB="libsfml-window.so"
    SFMLSYSTEMLIB="libsfml-system.so"

    EXECUTABLE="$PROJECTNAME-$BUILDNAME"

elif [ "$OSTYPE" == "darwin"* ]
then
    # Mac OSX
    echo "script running on mac osx"

    GLFWLIBNAME="libglfw.so"
    SFMLAUDIOLIB="libsfml-audio.so" 
    SFMLGRAPICSLIB="libsfml-graphics.so" 
    SFMLWINDOWLIB="libsfml-window.so"
    SFMLSYSTEMLIB="libsfml-system.so"

    EXECUTABLE="$PROJECTNAME-$BUILDNAME"
        
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]
then
    # Lightweight shell and GNU utilities compiled for Windows (part of MinGW)
    # or 
    # POSIX compatibility layer and Linux environment emulation for Windows
    echo "script running on windows"

    GLFWLIB="glfw3.dll"
    SFMLAUDIOLIB="sfml-audio-2.dll" 
    SFMLGRAPICSLIB="sfml-graphics-2.dll" 
    SFMLWINDOWLIB="sfml-window-2.dll"
    SFMLSYSTEMLIB="sfml-system-2.dll"

    EXECUTABLE="$PROJECTNAME-$BUILDNAME.exe"

    #alias make='mingw32-make'

else
    # Unknown os
    echo "running on something else."
    echo "Not a supported OS: $OSTYPE" >&2
    exit 1
fi

mkdir -p "build"

#compiling glfw
mkdir -p "build/glfw"
cd "build/glfw"
cmake -G "Unix Makefiles" -DBUILD_SHARED_LIBS=ON  "../../dependencies/glfw"
cmake --build .
if [ $? -eq 0 ]
then
  echo "Successfully compiled glfw"
else
  echo "Could not compile glfw" >&2
  cd "../.."
  exit 2
fi
cd "../.."

#compiling SFML
mkdir -p "build/SFML"
cd "build/SFML"
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="Release" -DBUILD_SHARED_LIBS=ON "../../dependencies/SFML"
make -j2
if [ $? -eq 0 ]
then
  echo "Successfully compiled SFML"
else
  echo "Could not compile SFML" >&2
  cd "../.."
  exit 3
fi
cd "../.."

rm -rf "dependencies/SFML/bin"

#copy results
mkdir -p "lib"
mkdir -p "build/$BUILDNAME"
mkdir -p "deploy"

cp "build/glfw/src/$GLFWLIBNAME" "lib"
cp "build/SFML/lib/$SFMLAUDIOLIB" "lib"
cp "build/SFML/lib/$SFMLGRAPICSLIB" "lib"
cp "build/SFML/lib/$SFMLWINDOWLIB" "lib"
cp "build/SFML/lib/$SFMLSYSTEMLIB" "lib"

cp "build/glfw/src/$GLFWLIBNAME" "build/$BUILDNAME"
cp "build/SFML/lib/$SFMLAUDIOLIB" "build/$BUILDNAME"
cp "build/SFML/lib/$SFMLGRAPICSLIB" "build/$BUILDNAME"
cp "build/SFML/lib/$SFMLWINDOWLIB" "build/$BUILDNAME"
cp "build/SFML/lib/$SFMLSYSTEMLIB" "build/$BUILDNAME"

cp "build/glfw/src/$GLFWLIBNAME" "deploy"
cp "build/SFML/lib/$SFMLAUDIOLIB" "deploy"
cp "build/SFML/lib/$SFMLGRAPICSLIB" "deploy"
cp "build/SFML/lib/$SFMLWINDOWLIB" "deploy"
cp "build/SFML/lib/$SFMLSYSTEMLIB" "deploy"

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
if [ $? -eq 0 ]
then
  echo "Successfully compiled $PROJECTNAME"
else
  echo "Could not compile $PROJECTNAME" >&2
  cd "../.."
  exit 3
fi
cd "../.."
cp -r "build/$BUILDNAME/$EXECUTABLE" "deploy"

LD_LIBRARY_PATH="$REPOROOT/lib"
export LD_LIBRARY_PATH
