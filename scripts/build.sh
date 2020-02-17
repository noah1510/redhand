#!/bin/bash

THREADS="3"
LIBBUILDNAME="lib"

#parse parameter
pars=$#
for ((i=1;i<=pars;i+=2))
do
  case "$1" in
    "-o")
      shift
      if [ $1 ]
      then
        LIBBUILDNAME="$1"
      else
        LIBBUILDNAME="lib"
      fi
      shift
      ;;
    "-j")
      shift
      if [ $1 ]
      then
        THREADS="$(($1+1))"
      fi
      shift
      ;;
    "--help")
      echo "This script is used to build the shared library and copy it in the deploy folder."
      echo "Usage: scripts/build.sh [options]"
      echo "Options:"
      echo "    --help              Display this information"
      echo "    -j [threadnumber]   Build the project with the specified number of threads."
      echo "    -o [buildname]      Build the project with the specified buildname defaults to main"
      echo ""
      echo "view the source on https://github.com/noah1510/redhand"
      exit 1
      ;;
    *)
      echo "Invalid option try --help for information on how to use the script"
      exit 1
      ;;
  esac
done

REPOROOT="$(pwd)"
PROJECTNAME="redhand"

if [ "$OSTYPE" == "linux-gnu" ]
then
    # Linux
    echo "script running on linux"

    LIBRARY="libredhand.so"

elif [ "$OSTYPE" == "darwin"* ]
then
    # Mac OSX
    echo "script running on mac osx"

    LIBRARY="libredhand.so"
        
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]
then
    # Lightweight shell and GNU utilities compiled for Windows (part of MinGW)
    # or 
    # POSIX compatibility layer and Linux environment emulation for Windows
    echo "script running on windows"

    LIBRARY="libredhand.dll"

    #alias make='mingw32-make'

else
    # Unknown os
    echo "running on something else."
    echo "Not a supported OS: $OSTYPE" >&2
    exit 1
fi

echo "number of THREADS:$THREADS"
echo "buildname:$LIBBUILDNAME"

mkdir -p "build"

#build actual project
mkdir -p "build/$LIBBUILDNAME"
cd "build/$LIBBUILDNAME"
cmake -G "Ninja" -DREPOROOT=$REPOROOT "../.."
ninja -j"$THREADS"
if [ $? -eq 0 ]
then
  echo "Successfully compiled $PROJECTNAME"
else
  echo "Could not compile $PROJECTNAME" >&2
  cd "../.."
  exit 4
fi
cd "../.."
cp -r "build/$LIBBUILDNAME/$LIBRARY" "deploy"
cp -r "build/$LIBBUILDNAME/$LIBRARY" "lib"

LD_LIBRARY_PATH="$REPOROOT/lib"
export LD_LIBRARY_PATH
