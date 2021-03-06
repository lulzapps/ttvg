#!/bin/bash
#
#
#

GIT_ROOT="$(git rev-parse --show-toplevel)"

if [ "$1" == "clean" ]; then
    rm -rf "$GIT_ROOT/build"
    exit 0
fi

if [ ! -d "$GIT_ROOT/build" ]; then
    mkdir "$GIT_ROOT/build"
fi

cd "$GIT_ROOT/build"

conan install .. -s build_type=Debug --build=missing
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .

