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
  echo "Successfully built"
else
  echo "Could not build" >&2
  cd "../.."
  set +x
  exit 4
fi

./scripts/run.sh
if [ $? -eq 0 ]
then
  echo "Successfully ran"
else
  echo "Could not run" >&2
  cd "../.."
  set +x
  exit 4
fi
