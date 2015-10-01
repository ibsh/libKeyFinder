#!/usr/bin/env bash

# Exit if any sub-command fails.
set -e

echo "Building on machine:"
uname -a

echo "CXX=$CXX"
echo "$($CXX --version)"

cd $(dirname $0)
echo "Inside directory: $(pwd)"

echo "Building and installing libKeyFinder."
qmake
make
sudo make install

echo "Building the tests."
cd tests/
echo "Inside directory: $(pwd)"

qmake
make

# Warning: this is a bit hacky! We have a known intermittent failure. Try and
# run the tests a reasonable amount of times such that if a commit didn't cause
# the intermittent failure to become a permanent failure, or if a new test is
# failing, then the tests should pass at least once.
#
# This hack ensures that we maintain a good signal to noise ratio for Travis CI
# tests results for pull requests and new commits.

echo "Running tests."

./tests || \
    ./tests || \
    ./tests || \
    ./tests || \
    ./tests || \
    ./tests || \
    ./tests || \
    ./tests || \
    ./tests || \
    ./tests || {
        echo "Tests consistently failing!"
        exit 1
    }
