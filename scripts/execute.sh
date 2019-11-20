#!/bin/bash

BUILDARGS=""
for i in "$@"
do
    BUILDARGS="$BUILDARGS $i"
done
./scripts/build.sh "$BUILDARGS"
./scripts/run.sh