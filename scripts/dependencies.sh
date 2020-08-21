#!/bin/bash

CI="0"

if [ "$1" == "--ci" ]
then
    CI="1"
fi

if [ "$REDHAND_CI" == "1" ]
then
    CI="1"
fi

if [ "$OSTYPE" == "linux-gnu" ]
then
    # Linux
    echo "script running on linux"

    DOCDEPS="doxygen graphviz-dev"
    GLFWDEPS="xorg-dev libgl1-mesa-dev"
    REDHANDDEPS="meson clang-10 clang-tools-10 ninja-build libglm-dev libglfw3-dev devscripts libvips-dev libclang-10-dev python3-glad libopenal-dev"
    ADDITIONALDEPS="python3-setuptools python-setuptools build-essential autoconf libtool pkg-config python-pil python-dev"

    sudo apt-get update
    if [ "$CI" == "1" ]
    then
        sudo apt-get install $DOCDEPS $GLFWDEPS $REDHANDDEPS --yes
        if [ $? -eq 0 ]
        then
        echo "Successfully installed dependencies"
        else
        echo "Could not install dependencies" >&2
        exit 2
        fi
    else
        sudo apt-get install $DOCDEPS $GLFWDEPS $REDHANDDEPS $ADDITIONALDEPS --yes
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
        
elif [[ "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]
then
    # Lightweight shell and GNU utilities compiled for Windows (part of MinGW)
    # or 
    # POSIX compatibility layer and Linux environment emulation for Windows
    echo "script running on windows"

    if [ "$CI" == "1" ]
    then
        #choco install imagemagick  -PackageParameters InstallDevelopmentHeaders=true  --yes --no-progress
        #if [ $? -eq 0 ]
        #then
        #    echo "Successfully installed magick"
        #else
        #    echo "Could not install magick" >&2
        #    exit 2
        #fi

        #choco install llvm --yes --no-progress
        #if [ $? -eq 0 ]
        #then
        #    echo "Successfully installed llvm"
        #else
        #    echo "Could not install llvm" >&2
        #    exit 2
        #fi

    else
        #choco install -PackageParameters InstallDevelopmentHeaders=true imagemagick
        #if [ $? -eq 0 ]
        #then
        #    echo "Successfully installed magick"
        #else
        #    echo "Could not install magick" >&2
        #    exit 2
        #fi

        choco install doxygen.install --yes --verbose --no-progress
        if [ $? -eq 0 ]
        then
            echo "Successfully installed doxygen"
        else
            echo "Could not install doxygen" >&2
            exit 2
        fi
    fi

    choco install  mingw pip llvm --yes --verbose --no-progress
    if [ $? -eq 0 ]
    then
        echo "Successfully installed dependencies"
    else
        echo "Could not install dependencies" >&2
        exit 2
    fi

    pip install meson glad
elif [ "$OSTYPE" == "msys" ]
then
    pacman -S --noconfirm pactoys git mingw-w64-x86_64-python-pip
    pacboy -S --noconfirm pkg-config:x clang:x opencv:x doxygen:x glfw:x glm:x graphviz:x libvips:x glib2:x openal:x meson:x
    pip install glad
else
    # Unknown os
    echo "running on something else."
    echo "Not a supported OS: $OSTYPE" >&2
    exit 1
fi

mkdir -p "lib"
mkdir -p "deploy"
