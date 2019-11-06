#!/bin/bash

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

    EXECUTABLE="$PROJECTNAME-$BUILDNAME"

elif [ "$OSTYPE" == "darwin"* ]
then
    # Mac OSX
    echo "script running on mac osx"

    EXECUTABLE="$PROJECTNAME-$BUILDNAME"
        
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]
then
    # Lightweight shell and GNU utilities compiled for Windows (part of MinGW)
    # or 
    # POSIX compatibility layer and Linux environment emulation for Windows
    echo "script running on windows"

    EXECUTABLE="$PROJECTNAME-$BUILDNAME.exe"

    #alias make='mingw32-make'

else
    # Unknown os
    echo "running on something else."
    echo "Not a supported OS: $OSTYPE" >&2
    exit 1
fi

cd "deploy"

#run the executable
./$EXECUTABLE

if [ $? -eq 0 ]
then
  echo "Successfully ran program"
else
  echo "Error while executing the program:" >&2
  cd ".."
  exit 2
fi

cd ".."
exit 0