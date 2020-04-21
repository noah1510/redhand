#!/bin/bash
set -x

if [ "$1" == "--setup" ]
then
  bash ./scripts/setup.sh
  if [ $? -eq 0 ]
  then
    echo "Successfully ran setup"
  else
    echo "setup failed" >&2
    cd "../.."
    set +x
    exit 10
  fi
fi

if [ "$1" == "--init" ]
then
  bash ./scripts/dependencies.sh
  if [ $? -eq 0 ]
  then
    echo "Successfully installed dependencies"
  else
    echo "could not install dependencies" >&2
    cd "../.."
    set +x
    exit 10
  fi

  bash ./scripts/setup.sh
  if [ $? -eq 0 ]
  then
    echo "Successfully ran setup"
  else
    echo "setup failed" >&2
    cd "../.."
    set +x
    exit 10
  fi
fi

bash ./scripts/build.sh
if [ $? -eq 0 ]
then
  echo "Successfully built library"
else
  echo "Could not build" >&2
  cd "../.."
  set +x
  exit 1
fi

bash ./testgame/scripts/build.sh
if [ $? -eq 0 ]
then
  echo "Successfully built testgame"
else
  echo "Could not build" >&2
  cd "../.."
  set +x
  exit 2
fi

bash ./testgame/scripts/run.sh
if [ $? -eq 0 ]
then
  echo "Successfully ran"
else
  echo "Could not run" >&2
  cd "../.."
  set +x
  exit 3
fi
