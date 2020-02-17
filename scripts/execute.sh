#!/bin/bash
set -x

BUILDARGS=$@
#for i in "$@"
#do
#    BUILDARGS="$BUILDARGS $i"
#done

./scripts/build.sh $BUILDARGS
if [ $? -eq 0 ]
then
  echo "Successfully built library"
else
  echo "Could not build" >&2
  cd "../.."
  set +x
  exit 1
fi

./scripts/testgame.sh $BUILDARGS
if [ $? -eq 0 ]
then
  echo "Successfully built testgame"
else
  echo "Could not build" >&2
  cd "../.."
  set +x
  exit 2
fi

./scripts/run.sh
if [ $? -eq 0 ]
then
  echo "Successfully ran"
else
  echo "Could not run" >&2
  cd "../.."
  set +x
  exit 3
fi
