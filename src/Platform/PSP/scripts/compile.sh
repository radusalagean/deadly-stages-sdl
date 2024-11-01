#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/../../parse-build-type.sh"
source "$(dirname "${BASH_SOURCE[0]}")/../../pre-run.sh"

FINAL_BUILD_DIR="build/psp-$BUILD_TYPE"

if [ "$BUILD_TYPE" = "release" ]; then
    CMAKE_BUILD_TYPE="Release"
else
    CMAKE_BUILD_TYPE="Debug"
fi

mkdir -p $FINAL_BUILD_DIR/cmake-output
psp-cmake -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE -B $FINAL_BUILD_DIR/cmake-output src/Platform/PSP
cd "$FINAL_BUILD_DIR"
make -C cmake-output

if [ "$BUILD_TYPE" = "release" ]; then
    # Create ISO file
    mkdir -p ISO_ROOT/PSP_GAME/SYSDIR
    mkdir -p ISO_ROOT/PSP_GAME/USRDIR
    cp DeadlyStagesRemix.prx ISO_ROOT/PSP_GAME/SYSDIR/EBOOT.BIN
    cp -r res ISO_ROOT/PSP_GAME/USRDIR/
    cp "$PROJECT_ROOT_DIR/src/Platform/PSP/ICON0.PNG" ISO_ROOT/PSP_GAME/ICON0.PNG
    cp "$PROJECT_ROOT_DIR/src/Platform/PSP/PIC1.PNG" ISO_ROOT/PSP_GAME/PIC1.PNG
    cp PARAM.SFO ISO_ROOT/PSP_GAME/PARAM.SFO
    ISO_VOLUME_NAME="DeadlyStagesRemixPSP"
    ISO_FILE_NAME="$ISO_VOLUME_NAME.iso"
    PLATFORM_IDENTIFIER="PSP GAME"
    PUBLISHER="Shadowz Games"
    mkisofs -iso-level 4 -xa -A "$PLATFORM_IDENTIFIER" -V "$ISO_VOLUME_NAME" \
        -sysid "$PLATFORM_IDENTIFIER" -volset "" -publisher "$PUBLISHER" \
        -o "$ISO_FILE_NAME" ISO_ROOT

    echo "Created $ISO_FILE_NAME"
    find . -mindepth 1 ! -name "DeadlyStagesRemixPSP.iso" -type f -exec rm {} +
    find . -mindepth 1 -type d -empty -delete
    echo "Cleaned up intermediate build files"
fi
