#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/../../Common/scripts/parse-build-type.sh"
source "$(dirname "${BASH_SOURCE[0]}")/../../Common/scripts/pre-run.sh"

FINAL_BUILD_DIR="build/windows-x86_64-$BUILD_TYPE"

COMMON_SOURCES=$(find src -name "*.cpp" -not -path "src/Platform/*")
PLATFORM_SPECIFIC_SOURCES="$(find src/Platform/Windows -name "*.cpp") src/Platform/Common/SaveDataHelper.cpp"

x86_64-w64-mingw32-g++ --std=c++17 \
    -DWIN32 \
    -static-libstdc++ -static-libgcc \
    $COMMON_SOURCES \
    $PLATFORM_SPECIFIC_SOURCES \
    vendor/tinyxml2/tinyxml2.cpp \
    -o $FINAL_BUILD_DIR/DeadlyStagesRemix.exe \
    -Wall \
    $(if [ "$BUILD_TYPE" = "release" ]; then echo "-O2"; else echo "-O0 -g"; fi) \
    -I$HOME/sdl2-devel-mingw/SDL2-devel-2.30.9-mingw/SDL2-2.30.9/x86_64-w64-mingw32/include/SDL2 \
    -L$HOME/sdl2-devel-mingw/SDL2-devel-2.30.9-mingw/SDL2-2.30.9/x86_64-w64-mingw32/lib \
    -I$HOME/sdl2-devel-mingw/SDL2_image-devel-2.8.2-mingw/SDL2_image-2.8.2/x86_64-w64-mingw32/include/SDL2 \
    -L$HOME/sdl2-devel-mingw/SDL2_image-devel-2.8.2-mingw/SDL2_image-2.8.2/x86_64-w64-mingw32/lib \
    -I$HOME/sdl2-devel-mingw/SDL2_mixer-devel-2.8.0-mingw/SDL2_mixer-2.8.0/x86_64-w64-mingw32/include/SDL2 \
    -L$HOME/sdl2-devel-mingw/SDL2_mixer-devel-2.8.0-mingw/SDL2_mixer-2.8.0/x86_64-w64-mingw32/lib \
    -I$HOME/sdl2-devel-mingw/SDL2_ttf-devel-2.22.0-mingw/SDL2_ttf-2.22.0/x86_64-w64-mingw32/include/SDL2 \
    -L$HOME/sdl2-devel-mingw/SDL2_ttf-devel-2.22.0-mingw/SDL2_ttf-2.22.0/x86_64-w64-mingw32/lib \
    -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf \
    -Ivendor/tinyxml2 \
    -mwindows

cp $HOME/sdl2-devel-mingw/SDL2-2.30.9-win32-x64/SDL2.dll $FINAL_BUILD_DIR/
cp $HOME/sdl2-devel-mingw/SDL2_image-2.8.2-win32-x64/SDL2_image.dll $FINAL_BUILD_DIR/
cp $HOME/sdl2-devel-mingw/SDL2_mixer-2.8.0-win32-x64/SDL2_mixer.dll $FINAL_BUILD_DIR/
cp $HOME/sdl2-devel-mingw/SDL2_mixer-2.8.0-win32-x64/optional/libogg-0.dll $FINAL_BUILD_DIR/
cp $HOME/sdl2-devel-mingw/SDL2_ttf-2.22.0-win32-x64/SDL2_ttf.dll $FINAL_BUILD_DIR/