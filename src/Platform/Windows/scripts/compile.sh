#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/../../Common/scripts/parse-build-type.sh"
source "$(dirname "${BASH_SOURCE[0]}")/../../Common/scripts/pre-run.sh"

FINAL_BUILD_DIR="build/windows-x86_64-$BUILD_TYPE"

COMMON_SOURCES=$(find src -name "*.cpp" -not -path "src/Platform/*")
PLATFORM_SPECIFIC_SOURCES=$(find src/Platform/Windows -name "*.cpp")

x86_64-w64-mingw32-g++ --std=c++17 \
    -static-libstdc++ -static-libgcc \
    $COMMON_SOURCES \
    $PLATFORM_SPECIFIC_SOURCES \
    vendor/tinyxml2/tinyxml2.cpp \
    -o $FINAL_BUILD_DIR/DeadlyStagesRemix.exe \
    -Wall \
    $(if [ "$BUILD_TYPE" = "release" ]; then echo "-O2"; else echo "-O0 -g"; fi) \
    -I/home/radu/Desktop/SDL2-devel-2.30.8-mingw/SDL2-2.30.8/x86_64-w64-mingw32/include/SDL2 \
    -L/home/radu/Desktop/SDL2-devel-2.30.8-mingw/SDL2-2.30.8/x86_64-w64-mingw32/lib \
    -I/home/radu/Desktop/SDL2_image-devel-2.8.2-mingw/SDL2_image-2.8.2/x86_64-w64-mingw32/include/SDL2 \
    -L/home/radu/Desktop/SDL2_image-devel-2.8.2-mingw/SDL2_image-2.8.2/x86_64-w64-mingw32/lib \
    -I/home/radu/Desktop/SDL2_mixer-devel-2.8.0-mingw/SDL2_mixer-2.8.0/x86_64-w64-mingw32/include/SDL2 \
    -L/home/radu/Desktop/SDL2_mixer-devel-2.8.0-mingw/SDL2_mixer-2.8.0/x86_64-w64-mingw32/lib \
    -I/home/radu/Desktop/SDL2_ttf-devel-2.22.0-mingw/SDL2_ttf-2.22.0/x86_64-w64-mingw32/include/SDL2 \
    -L/home/radu/Desktop/SDL2_ttf-devel-2.22.0-mingw/SDL2_ttf-2.22.0/x86_64-w64-mingw32/lib \
    -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf \
    -Ivendor/tinyxml2 \
    -mwindows