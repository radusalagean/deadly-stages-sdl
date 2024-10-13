#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/pre-run.sh"

COMMON_SOURCES=$(find src -name "*.cpp" -not -path "src/Platform/*")
PLATFORM_SPECIFIC_SOURCES=$(find src/Platform/macOS -name "*.cpp")

clang++ --std=c++17 \
    $COMMON_SOURCES \
    $PLATFORM_SPECIFIC_SOURCES \
    vendor/tinyxml2/tinyxml2.cpp \
    -o build/macos-arm64-debug/DeadlyStagesRemix \
    -Wall -O0 -g \
    `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf \
    -Ivendor/tinyxml2
