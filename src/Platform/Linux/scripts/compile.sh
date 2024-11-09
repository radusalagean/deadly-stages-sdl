#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/../../Common/scripts/parse-build-type.sh"
source "$(dirname "${BASH_SOURCE[0]}")/../../Common/scripts/pre-run.sh"

make -f src/Platform/Linux/Makefile BUILD_TYPE=$BUILD_TYPE

if [ "$BUILD_TYPE" = "release" ]; then
    EXECUTABLE=build/linux-x86_64-$BUILD_TYPE/DeadlyStagesRemix
    LIB_DIR=build/linux-x86_64-$BUILD_TYPE/lib

    mkdir -p "$LIB_DIR"

    ldd "$EXECUTABLE" | awk '{ print $3 }' | grep -v '^$' | while read LIB; do
        if [[ "$(basename "$LIB")" != "libstdc++.so.6" && 
              "$(basename "$LIB")" != "libm.so.6" && 
              "$(basename "$LIB")" != "libgcc_s.so.1" && 
              "$(basename "$LIB")" != "libc.so.6" ]]; then
            cp "$LIB" "$LIB_DIR" || true  
        fi
    done

    echo "Libraries copied to $LIB_DIR"
fi
