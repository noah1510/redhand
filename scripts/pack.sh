#!/bin/bash

ZIPNAME=$1

if [ "$OSTYPE" == "linux-gnu" ]
then
    # Linux
    echo "script running on linux"
    echo "name of the zip:$ZIPNAME"

    zip deploy/$ZIPNAME deploy/*.so scripts/run.sh shaders

elif [ "$OSTYPE" == "darwin"* ]
then
    # Mac OSX
    echo "script running on mac osx"

        
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]
then
    # Lightweight shell and GNU utilities compiled for Windows (part of MinGW)
    # or 
    # POSIX compatibility layer and Linux environment emulation for Windows
    echo "script running on windows"



else
    # Unknown os
    echo "running on something else."
    echo "Not a supported OS: $OSTYPE" >&2
    exit 1
fi
