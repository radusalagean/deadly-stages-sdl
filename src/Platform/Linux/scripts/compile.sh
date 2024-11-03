#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/../../Common/scripts/parse-build-type.sh"
source "$(dirname "${BASH_SOURCE[0]}")/../../Common/scripts/pre-run.sh"

FINAL_BUILD_DIR="build/linux-x86_64-$BUILD_TYPE"

COMMON_SOURCES=$(find src -name "*.cpp" -not -path "src/Platform/*")
PLATFORM_SPECIFIC_SOURCES=$(find src/Platform/Linux -name "*.cpp")
PLATFORM_SPECIFIC_SOURCES+=" src/Platform/Common/SaveDataHelper.cpp"

g++ --std=c++17 \
    $COMMON_SOURCES \
    $PLATFORM_SPECIFIC_SOURCES \
    vendor/tinyxml2/tinyxml2.cpp \
    -o $FINAL_BUILD_DIR/DeadlyStagesRemix \
    -Wall \
    $(if [ "$BUILD_TYPE" = "release" ]; then echo "-O2"; else echo "-O0 -g"; fi) \
    `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf \
    -Ivendor/tinyxml2
