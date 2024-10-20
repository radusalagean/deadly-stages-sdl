#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/pre-run.sh"

mkdir -p build/psp-debug/cmake-output
psp-cmake -DCMAKE_BUILD_TYPE=Debug -B build/psp-debug/cmake-output src/Platform/PSP
cd "$PROJECT_ROOT_DIR/build/psp-debug"
make -C cmake-output
