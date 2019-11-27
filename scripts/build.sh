#!/bin/bash

THREATS="3"
#parse parameter
if [ $1 ]
then
  if [ $1 = "-j" ]
  then
    THREATS="$(($2+1))"
    if [ $3 ]
    then
      BUILDNAME="$3"
    else
      BUILDNAME="main"
    fi #[ $3 ]
  else
    BUILDNAME="$1"
  fi #[ $1 -eq "-j" ]
else 
    BUILDNAME="main"
fi #[ $1 ]

REPOROOT="$(pwd)"
PROJECTNAME="greenfoot++"

if [ "$OSTYPE" == "linux-gnu" ]
then
    # Linux
    echo "script running on linux"

    GLFWLIB="libglfw.so"

    SOURCESFMLAUDIOLIB="libsfml-audio.so.2.5" 
    SOURCESFMLGRAPICSLIB="libsfml-graphics.so.2.5" 
    SOURCESFMLWINDOWLIB="libsfml-window.so.2.5"
    SOURCESFMLSYSTEMLIB="libsfml-system.so.2.5"

    SFMLAUDIOLIB="libsfml-audio.so" 
    SFMLGRAPICSLIB="libsfml-graphics.so" 
    SFMLWINDOWLIB="libsfml-window.so"
    SFMLSYSTEMLIB="libsfml-system.so"

    EXECUTABLE="$PROJECTNAME-$BUILDNAME"

elif [ "$OSTYPE" == "darwin"* ]
then
    # Mac OSX
    echo "script running on mac osx"

    GLFWLIB="libglfw.so"
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

    SOURCESFMLAUDIOLIB="sfml-audio-2.dll" 
    SOURCESFMLGRAPICSLIB="sfml-graphics-2.dll" 
    SOURCESFMLWINDOWLIB="sfml-window-2.dll"
    SOURCESFMLSYSTEMLIB="sfml-system-2.dll"

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

echo "number of threats:$THREATS"

mkdir -p "build"

#compiling glfw
mkdir -p "build/glfw"
cd "build/glfw"
#cmake -G "Unix Makefiles" -DBUILD_SHARED_LIBS=ON  "../../dependencies/glfw"
#make -j"$THREATS"
cmake -G "Ninja" -DBUILD_SHARED_LIBS=ON  "../../dependencies/glfw"
ninja -j"$THREATS"
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
#cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="Release" -DBUILD_SHARED_LIBS=ON "../../dependencies/SFML"
#make -j"$THREATS"
cmake -G "Ninja" -DCMAKE_BUILD_TYPE="Release" -DBUILD_SHARED_LIBS=ON "../../dependencies/SFML"
ninja -j"$THREATS"
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

cp "build/glfw/src/$GLFWLIB" "lib"

cp "build/SFML/lib/$SOURCESFMLAUDIOLIB" "lib/$SOURCESFMLAUDIOLIB"
cp "build/SFML/lib/$SOURCESFMLGRAPICSLIB" "lib/$SOURCESFMLGRAPICSLIB"
cp "build/SFML/lib/$SOURCESFMLWINDOWLIB" "lib/$SOURCESFMLWINDOWLIB"
cp "build/SFML/lib/$SOURCESFMLSYSTEMLIB" "lib/$SOURCESFMLSYSTEMLIB"

cp "build/SFML/lib/$SOURCESFMLAUDIOLIB" "lib/$SFMLAUDIOLIB"
cp "build/SFML/lib/$SOURCESFMLGRAPICSLIB" "lib/$SFMLGRAPICSLIB"
cp "build/SFML/lib/$SOURCESFMLWINDOWLIB" "lib/$SFMLWINDOWLIB"
cp "build/SFML/lib/$SOURCESFMLSYSTEMLIB" "lib/$SFMLSYSTEMLIB"


cp "build/glfw/src/$GLFWLIB" "build/$BUILDNAME"
cp "build/SFML/lib/$SOURCESFMLAUDIOLIB" "build/$BUILDNAME"
cp "build/SFML/lib/$SOURCESFMLGRAPICSLIB" "build/$BUILDNAME"
cp "build/SFML/lib/$SOURCESFMLWINDOWLIB" "build/$BUILDNAME"
cp "build/SFML/lib/$SOURCESFMLSYSTEMLIB" "build/$BUILDNAME"

cp "build/glfw/src/$GLFWLIB" "deploy"
cp "build/SFML/lib/$SOURCESFMLAUDIOLIB" "deploy"
cp "build/SFML/lib/$SOURCESFMLGRAPICSLIB" "deploy"
cp "build/SFML/lib/$SOURCESFMLWINDOWLIB" "deploy"
cp "build/SFML/lib/$SOURCESFMLSYSTEMLIB" "deploy"

cp -r "dependencies/SFML/include/SFML" "include"

cp "deploy/$GLFWLIB" "deploy/$BUILDNAME-$GLFWLIB"
cp "deploy/$SOURCESFMLAUDIOLIB" "deploy/$BUILDNAME-$SFMLAUDIOLIB"
cp "deploy/$SOURCESFMLGRAPICSLIB" "deploy/$BUILDNAME-$SFMLGRAPICSLIB"
cp "deploy/$SOURCESFMLWINDOWLIB" "deploy/$BUILDNAME-$SFMLWINDOWLIB"
cp "deploy/$SOURCESFMLSYSTEMLIB" "deploy/$BUILDNAME-$SFMLSYSTEMLIB"

#build actual project
cd "build/$BUILDNAME"
#cmake -G "Unix Makefiles" -DOUTPUTFILE="$PROJECTNAME-$BUILDNAME" -DREPOROOT=$REPOROOT "../.."
#make -j"$THREATS"
cmake -G "Ninja" -DOUTPUTFILE="$PROJECTNAME-$BUILDNAME" -DREPOROOT=$REPOROOT "../.."
ninja -j"$THREATS"
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
