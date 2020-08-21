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
    echo "invalid directory $(pwd)"
    exit 1
  fi
}

BASPATHNAME="redhand"
THREADS="3"
CI="0"
VSCODE="0"
PACKAGEBUILD="0"
NOTESTGAME="0"

#parse parameter
pars=$#
for ((i=1;i<=pars;i+=1))
do
  case "$1" in
    "-j")
      shift
      if [ $1 ]
      then
        THREADS="$(($1+1))"
      fi
      shift
      i+=1
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
    "--ci")
      CI="1"
      shift
      ;;
    "--vscode")
      VSCODE="1"
      shift
      ;;
    "--deb")
      PACKAGEBUILD="1"
      shift
      ;;
    "--no-testgame")
      NOTESTGAME="1"
      shift
      ;;
    "--help")
      echo "Usage: scripts/build.sh [options]"
      echo "Options:"
      echo "    --help              Display this information"
      echo "    --ci                To run in CI mode to disable git-lfs pull."
      echo "    -j [threadnumber]   Build the project with the specified number of threads."
      echo "    --vscode            Addes the configurations for visual studio code to the .vscode folder"
      echo "    --deb               The setup to create debian packages (!!!only intended for the debhelper!!!)"
      echo "    --no-testgame       Do not setup the testgame"
      echo "    --pathname [name]   The name of the redhand folder"
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

if [ "$REDHAND_CI" == "1" ]
then
    CI="1"
fi

if [ "$THREADS" == "3" ]
then
  THREADS="$(nproc)"
  THREADS="$(($THREADS+1))"
fi

PROJECTNAME="redhand"
findReporoot

if [ "$OSTYPE" == "linux-gnu" ]
then
    # Linux
    echo "script running on linux"

    if [ $VSCODE -eq "1" ]
    then
      cp -r "configurations/linux/.vscode" "."
    fi


elif [ "$OSTYPE" == "darwin"* ]
then
    # Mac OSX
    echo "script running on mac osx"

    if [ $VSCODE -eq "1" ]
    then
      echo "The configurations are for linux and not guaranteed to work"
      cp -r "configurations/linux/.vscode" "."
    fi


elif [[ "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]
then
    # Lightweight shell and GNU utilities compiled for Windows (part of MinGW)
    # or
    # POSIX compatibility layer and Linux environment emulation for Windows
    echo "script running on windows"

    if [ $VSCODE -eq "1" ]
    then
      echo "The configurations are for linux and not guaranteed to work"
      cp -r "configurations/linux/.vscode" "."
    fi

elif [  "$OSTYPE" == "msys" ]
then
    echo "script running on windows"

else
    # Unknown os
    echo "running on something else."
    echo "Not a supported OS: $OSTYPE" >&2
    exit 1
fi

echo "number of THREADS:$THREADS"

if [ "$NOTESTGAME" == "0" ]
then

  if [ "$PACKAGEBUILD" == "0" ]
  then
      git submodule update --init testgame
      if [ $? -eq 0 ]
      then
          echo "Successfully initiated testgame"
      else
          echo "Could not initiate testgame" >&2
          exit 1
      fi
  else
      git clone https://github.com/noah1510/redhand-testgame.git testgame
      if [ $? -eq 0 ]
      then
          echo "Successfully initiated testgame"
      else
          echo "Could not initiate testgame" >&2
          exit 1
      fi
  fi

fi

echo "Successfully finished setup"
exit 0
