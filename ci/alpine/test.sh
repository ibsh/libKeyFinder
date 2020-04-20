#!/bin/sh
set -e

cd /outside
rm -rf build
mkdir -p build
cmake -S . -B build
cmake --build build
LD_LIBRARY_PATH=".." ./build/tests/test
LD_LIBRARY_PATH=".." valgrind \
    --suppressions=/outside/ci/alpine/memcheck.supp \
    --tool=memcheck ./build/tests/test
