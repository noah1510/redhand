#!/bin/bash

# get cross-distro version of POSIX command
COMMAND=""
if command -v command 2>/dev/null; then
    COMMAND="command -v"
elif type type 2>/dev/null; then
    COMMAND="type"
fi

for comp in clang-12 clang-11 clang-10 clang-9 gcc-10 gcc-9 C:/Program Files/LLVM/bin/clang.exe; do
    if $COMMAND "$comp" 2>/dev/null; then
        CC="$comp"
        CMAKE_C_COMPILER="$comp"
        export CC
        export CMAKE_C_COMPILER

        break;
    fi
done

for comp in clang++-12 clang++-11 clang++-10 clang++-9 g++-10 g++-9 C:/Program Files/LLVM/bin/clang++.exe; do
    if $COMMAND "$comp" 2>/dev/null; then
        CXX="$comp"
        CMAKE_CXX_COMPILER="$comp"
        export CXX
        export CMAKE_CXX_COMPILER

        break;
    fi
done
