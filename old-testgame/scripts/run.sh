#!/bin/bash
set -x

if [ $1 ]
then
    BUILDNAME="$1"
else 
    BUILDNAME="testgame"
fi

REPOROOT="$(pwd)"
PROJECTNAME="redhand"

if [ "$OSTYPE" == "linux-gnu" ]
then
    # Linux
    echo "script running on linux"

    export LD_LIBRARY_PATH="$(pwd)/deploy:$LD_LIBRARY_PATH"
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
    set +x
    exit 1
fi

#run the executable
./deploy/$EXECUTABLE | tail -l

if [ $? -eq ${PIPESTATUS[0]} ]
then
  echo "Successfully ran program"
else
  echo "Error during execution of the program: ${PIPESTATUS[0]} " >&2
  set +x
  exit 2
fi

set +x
exit 0