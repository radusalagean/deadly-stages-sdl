#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/pre-run.sh"

mkdir -p build/psp-release/cmake-output
psp-cmake -DCMAKE_BUILD_TYPE=Release -B build/psp-release/cmake-output src/Platform/PSP
cd "$PROJECT_ROOT_DIR/build/psp-release"
make -C cmake-output

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

# TODO: Remove if not needed
# mkisofs -U -xa -A "$PLATFORM_IDENTIFIER" -V "$ISO_VOLUME_NAME" \
#     -sysid "$PLATFORM_IDENTIFIER" -volset "" -p "" -publisher "$PUBLISHER" \
#     -max-iso9660-filenames -relaxed-filenames -pad \
#     -o "$ISO_FILE_NAME" ISO_ROOT

echo "Created $ISO_FILE_NAME"
find . -mindepth 1 ! -name "DeadlyStagesRemixPSP.iso" -type f -exec rm {} +
find . -mindepth 1 -type d -empty -delete
echo "Cleaned up intermediate build files"
