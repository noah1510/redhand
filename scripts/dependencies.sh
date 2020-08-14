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
    REDHANDDEPS="cmake clang-10 clang-tools-10 ninja-build libglm-dev libglfw3-dev devscripts libvips-dev libclang-10-dev libopenal-dev python3-glad"
    ADDITIONALDEPS="python3-setuptools python-setuptools build-essential autoconf libtool pkg-config python-pil python-dev"
    QT4DEPS="libqtgui4 libqtcore4 libqt4-xml libqt4-test libqt4-script libqt4-network libqt4-dbus python-qt4"
    OPENAL="libasound2 alsa-utils alsa-oss"

    sudo apt-get update
    if [ "$CI" == "1" ]
    then
        sudo apt-get install $DOCDEPS $GLFWDEPS $REDHANDDEPS $OPENAL --yes
        if [ $? -eq 0 ]
        then
        echo "Successfully installed dependencies"
        else
        echo "Could not install dependencies" >&2
        exit 2
        fi
    else
        sudo apt-get install $DOCDEPS $GLFWDEPS $REDHANDDEPS $ADDITIONALDEPS $OPENAL --yes
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
        
        choco install ninja --yes --no-progress
        if [ $? -eq 0 ]
        then
            echo "Successfully installed ninja"
        else
            echo "Could not install ninja" >&2
            exit 2
        fi

        choco install llvm --yes --no-progress
        if [ $? -eq 0 ]
        then
            echo "Successfully installed llvm"
        else
            echo "Could not install llvm" >&2
            exit 2
        fi

    else
        #choco install -PackageParameters InstallDevelopmentHeaders=true imagemagick
        #if [ $? -eq 0 ]
        #then
        #    echo "Successfully installed magick"
        #else
        #    echo "Could not install magick" >&2
        #    exit 2
        #fi

        choco install doxygen.install mingw cmake ninja llvm --yes --verbose --no-progress
        if [ $? -eq 0 ]
        then
            echo "Successfully installed dependencies"
        else
            echo "Could not install dependencies" >&2
            exit 2
        fi
    fi
elif [ "$OSTYPE" == "msys" ]
then
    pacman -S --noconfirm pactoys git
    pacboy -S --noconfirm ninja:x clang:x cmake:x opencv:x doxygen:x glfw:x glm:x graphviz:x libvips:x glib2:x openal:x
else
    # Unknown os
    echo "running on something else."
    echo "Not a supported OS: $OSTYPE" >&2
    exit 1
fi
