#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/pre-run.sh"

COMMON_SOURCES=$(find src -name "*.cpp" -not -path "src/Platform/*")
PLATFORM_SPECIFIC_SOURCES=$(find src/Platform/Linux -name "*.cpp")

g++ --std=c++17 \
    $COMMON_SOURCES \
    $PLATFORM_SPECIFIC_SOURCES \
    -o build/linux-x86_64-debug/DeadlyStagesDemo \
    -Wall -O0 -g \
    `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_mixer
