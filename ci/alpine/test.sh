#!/bin/sh
set -e

cd /outside
patch -p1 < ci/alpine/settings.patch
qmake
make
cd tests
ln -s /outside keyfinder
qmake
make
LD_LIBRARY_PATH=".." ./tests
LD_LIBRARY_PATH=".." valgrind \
    --suppressions=/outside/ci/alpine/memcheck.supp \
    --tool=memcheck ./tests
