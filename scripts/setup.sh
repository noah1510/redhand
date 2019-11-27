#!/bin/bash

git submodule update --init
if [ $? -eq 0 ]
then
    echo "Successfully initiated submodules"
else
    echo "Could not initiate submodules" >&2
    exit 1
fi

git lfs install
if [ $? -eq 0 ]
then
    echo "Successfully initiated git-lfs"
else
    echo "Could not initiate git-lfs" >&2
    exit 2
fi

echo "Successfully finished setup"
exit 0