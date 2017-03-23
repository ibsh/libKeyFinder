#!/bin/sh

rm -f ci/alpine/.cache/APKINDEX.*
SIZE="$(du -s ci/alpine/.cache | sed 's/\t.*//')"
if [ "$SIZE" -gt "280000" ]; then
    echo Cleaning cache
    rm -rf ci/alpine/.cache
    mkdir -p ci/alpine/.cache
    touch ci/alpine/.cache/.keep
fi
