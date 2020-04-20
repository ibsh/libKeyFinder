#!/bin/sh

SCRIPT="$(pwd -P)/$0"
BASE="${SCRIPT%/*}"
echo Running alpine docker test at $BASE
cd "$BASE"

if [ -z "$CC" ]; then
    echo Set CC=gcc since CC is undefined
    export CC=gcc
fi

sudo docker run \
    --rm \
    -e "CC=$CC" \
    -v "$(pwd -P)/..":/outside \
    alpine:edge \
    /outside/ci/alpine/run.sh
