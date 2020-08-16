#!/bin/bash

findReporoot(){
  if [ "$(basename $(pwd))" == "scripts" ]
  then
    cd ..
  fi

  if [ "$(basename $(pwd))" == "$BASPATHNAME" ]
  then
    REPOROOT="$(pwd)"
  else
    echo "invalid directory"
    exit 1
  fi
}

BASPATHNAME="redhand"
THREADS="3"
LIBBUILDNAME="lib"
SETUP="0"
PACKAGE="0"
INITILIZE="0"
BUILDGLFW="0"
export REDHAND_CI="0"
AUTOSETUP="1"

#parse parameter
pars=$#
for ((i=1;i<=pars;i+=1))
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
      i++
      ;;
    "--pathname")
      shift
      if [ $1 ]
      then
        BASPATHNAME="$1"
      fi
      shift
      i+=1
      ;;
    "-j")
      shift
      if [ $1 ]
      then
        THREADS="$(($1+1))"
      fi
      shift
      i+=1
      ;;
    "--setup")
      shift
      SETUP="1"
      ;;
    "--init")
      shift
      SETUP="1"
      INITILIZE="1"
      ;;
    "--deb")
      shift
      PACKAGE="1"
      ;;
    "--no-autosetup")
      shift
      AUTOSETUP="0"
      ;;
    "--ci")
      shift
      REDHAND_CI="1"
      export REDHAND_CI
      ;;
    "--help")
      echo "This script is used to build the shared library and copy it in the deploy folder."
      echo "Usage: scripts/build.sh [options]"
      echo "Options:"
      echo "    --help              Display this information"
      echo "    -j [threadnumber]   Build the project with the specified number of threads."
      echo "    -o [buildname]      Build the project with the specified buildname defaults to main"
      echo "    --pathname [name]   The name of the redhand folder"
      echo "    --setup             Also execute the setup script to prepare all dependencies"
      echo "    --init              Also execute the setup and dependencies scripts to prepare and install all dependencies"
      echo "    --deb               Build the library as debian package"
      echo "    --ci                Build the library in an CI (manually sets the CC and CXX for win)"
      echo "    --no-autosetup      Disable the automatic call of setup.sh in the CMakelist.txt"
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

if [ "$THREADS" == "3" ]
then
  THREADS="$(nproc)"
  THREADS="$(($THREADS+1))"
fi

findReporoot
PROJECTNAME="redhand"

if [ "$INITILIZE" == "1" ]
then
    bash ./scripts/dependencies.sh
fi

if [ "$SETUP" == "1" ]
then
    bash ./scripts/setup.sh
fi

if [ "$OSTYPE" == "linux-gnu" ]
then
    # Linux
    echo "script running on linux"

    LIBRARY="libredhand.so"
    BUILDGLFW="0"

elif [ "$OSTYPE" == "darwin"* ]
then
    # Mac OSX
    echo "script running on mac osx"

    LIBRARY="libredhand.so"

elif [[ "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]
then
    # Lightweight shell and GNU utilities compiled for Windows (part of MinGW)
    # or 
    # POSIX compatibility layer and Linux environment emulation for Windows
    echo "script running on windows"

    LIBRARY="libredhand.dll"

    BUILDGLFW="1"

elif [ "$OSTYPE" == "msys" ]
then
    echo "script running on windows"

    LIBRARY="libredhand.dll"

    BUILDGLFW="0"

else
    # Unknown os
    echo "running on something else."
    echo "Not a supported OS: $OSTYPE" >&2
    exit 1
fi

source ./scripts/configure_compiler.sh

echo "number of THREADS:$THREADS"
echo "buildname:$LIBBUILDNAME"

if [ "$PACKAGE" == "1" ]
then

  debuild -sa

else

  mkdir -p "build"

  #build actual project

  cmake -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B$REPOROOT/build -S$REPOROOT -DAUTOSETUP=$AUTOSETUP
  
  cd build
  ninja -j"$THREADS"
  if [ $? -eq 0 ]
  then
    echo "Successfully compiled $PROJECTNAME"
    cd ".."
  else
    echo "Could not compile $PROJECTNAME" >&2
    cd ".."
    exit 4
  fi
  
  cp -r "build/$LIBRARY" "deploy"
  cp -r "build/$LIBRARY" "lib"

  LD_LIBRARY_PATH="$REPOROOT/lib"
  export LD_LIBRARY_PATH
fi
