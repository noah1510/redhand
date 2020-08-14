#!/bin/bash

if [ "$(basename $(pwd))" == "scripts" ]
then
    cd ..
fi

if [ "$OSTYPE" == "linux-gnu" ]
then
    # Linux
    echo "script running on linux"

    if [ "$1" == "--init" ]
    then
        sudo apt-get update
        sudo apt-get install doxygen graphviz texlive-base texlive-latex-extra texlive-fonts-extra texlive-fonts-recommended --yes
        if [ $? -eq 0 ]
        then
            echo "Successfully installed dependencies"
        else
            echo "Could not install dependencies" >&2
            exit 2
        fi

        pip3 install jinja2 Pygments
        if [ $? -eq 0 ]
        then
            echo "Successfully installed dependencies"
        else
            echo "Could not install dependencies" >&2
            exit 2
        fi
    fi

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

    if [ $1 == "--init" ]
    then

        if [ "$2" == "--ci" ]
        then
            choco install doxygen.install --yes --verbose --no-progress
            if [ $? -eq 0 ]
            then
                echo "Successfully installed dependencies"
            else
                echo "Could not install dependencies" >&2
                exit 2
            fi
        else
            choco install doxygen.install --yes --verbose --no-progress
            if [ $? -eq 0 ]
            then
                echo "Successfully installed dependencies"
            else
                echo "Could not install dependencies" >&2
                exit 2
            fi
        fi
    fi

else
    # Unknown os
    echo "running on something else."
    echo "Not a supported OS: $OSTYPE" >&2
    exit 1
fi

if [ $1 == "--deb" ]
then
    doxygen
else
    git submodule update --init doc/m.css

    mkdir -p "doc/html"
    cd doc/m.css/documentation
    ./doxygen.py --debug ../../conf.py
    if [ $? -eq 0 ]
    then
        echo "Successfully installed dependencies"
    else
        echo "Could not install dependencies" >&2
        exit 2
    fi
fi
exit 0
