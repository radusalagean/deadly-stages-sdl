#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/pre-run.sh"

psp-cmake -DCMAKE_BUILD_TYPE=Debug -B build/psp-debug src/Platform/PSP
echo $PROJECT_ROOT_DIR
cd "$PROJECT_ROOT_DIR/build/psp-debug"
make