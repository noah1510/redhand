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

lookForGlad(){
  if [ -n "$(glad 2>&1 >/dev/null | grep -c "ModuleNotFoundError: No module named")" ]
  then
    if [ -n "$(python3 -m glad 2>&1 >/dev/null | grep -c "No module named glad")" ]
    then
      echo "System glad not installed"
    else
      SYSTEMGLAD="1"
    fi
  else
    SYSTEMGLAD="1"
  fi
}

BASPATHNAME="redhand"
THREADS="3"
CI="0"
VSCODE="0"
BUILDGLFW="0"
BUILDGLM="0"
PACKAGEBUILD="0"
NOTESTGAME="0"
SYSTEMGLAD="0"

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
      SYSTEMGLAD="1"
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
lookForGlad

if [ "$OSTYPE" == "linux-gnu" ]
then
    # Linux
    echo "script running on linux"

    GLFWLIB="libglfw.so"
    GLFWDEPLOY="libglfw.so.3"

    if [ $VSCODE -eq "1" ]
    then
      cp -r "configurations/linux/.vscode" "."
    fi


elif [ "$OSTYPE" == "darwin"* ]
then
    # Mac OSX
    echo "script running on mac osx"

    GLFWLIB="libglfw.so"

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

    GLFWLIB="glfw3.dll"
    GLFWDEPLOY="glfw3.dll"
    BUILDGLFW="1"
    BUILDGLM="1"

    if [ $VSCODE -eq "1" ]
    then
      echo "The configurations are for linux and not guaranteed to work"
      cp -r "configurations/linux/.vscode" "."
    fi

elif [  "$OSTYPE" == "msys" ]
then
    echo "script running on windows"
    
    BUILDGLFW="0"
    BUILDGLM="0"
else
    # Unknown os
    echo "running on something else."
    echo "Not a supported OS: $OSTYPE" >&2
    exit 1
fi

echo "number of THREADS:$THREADS"

if [ "$BUILDGLM" == "1" ]
then
  git submodule update --init include/gitglm
  if [ $? -eq 0 ]
  then
    echo "Successfully initiated glm"
  else
    echo "Could not initiate glm" >&2
    exit 1
  fi
fi

if [ "$BUILDGLFW" == "1" ]
then
  git submodule update --init dependencies/glfw
  if [ $? -eq 0 ]
  then
    echo "Successfully initiated glfw"
  else
    echo "Could not initiate glfw" >&2
    exit 1
  fi
fi

if [ "$PACKAGEBUILD" == "0" ]
then
    if [ "$SYSTEMGLAD" == "0" ]
    then

      git submodule update --init dependencies/gladRepo
      if [ $? -eq 0 ]
      then
          echo "Successfully initiated glad"
      else
          echo "Could not initiate glad" >&2
          exit 1
      fi

    fi
    
    cd "dependencies/gladRepo"
    python3 -m glad --generator=c --extensions=GL_EXT_framebuffer_multisample,GL_EXT_texture_filter_anisotropic --out-path="../glad" --reproducible --profile core
    if [ $? -eq 0 ]
    then
      echo "Successfully initilized glad"
    else
      echo "Could not initilize glad" >&2
      cd "../.."
      exit 3
    fi
    cd "../.."

fi

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

if [ "$BUILDGLM" == "1" ]
then
  cp -r "include/gitglm/glm" "include/glm"
  if [ $? -eq 0 ]
  then
      echo "Successfully copied glm"
  else
      echo "Could not copy glm" >&2
      exit 2
  fi
fi

if [ "$BUILDGLFW" == "1" ]
then
  cp -r "dependencies/glfw/include/GLFW" "include/GLFW"
  if [ $? -eq 0 ]
  then
      echo "Successfully copied GLFW"
  else
      echo "Could not copy GLFW" >&2
      exit 2
  fi
fi

mkdir -p "build"
mkdir -p "lib"
mkdir -p "build/$BUILDNAME"
mkdir -p "deploy"

#copy results
cp -r "dependencies/glad/include" "."
mkdir -p "include/redhand/glad"
mkdir -p "include/redhand/KHR"
cp -r "dependencies/glad/include/glad/glad.h" "include/redhand/glad"
cp -r "dependencies/glad/include/KHR/khrplatform.h" "include/redhand/KHR"
cp -r "dependencies/glad/src/glad.c" "src"

echo "Successfully finished setup"
exit 0
