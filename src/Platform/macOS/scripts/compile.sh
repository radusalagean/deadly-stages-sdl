#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/../../Common/scripts/parse-build-type.sh"
source "$(dirname "${BASH_SOURCE[0]}")/../../Common/scripts/pre-run.sh"

make -f src/Platform/macOS/Makefile BUILD_TYPE=$BUILD_TYPE

if [ "$BUILD_TYPE" = "release" ]; then
    BASE_DIR=build/macos-arm64-$BUILD_TYPE
    APP_DIR="$BASE_DIR/Deadly Stages Remix.app"

    mkdir -p "$APP_DIR/Contents/MacOS"
    mkdir -p "$APP_DIR/Contents/Resources"
    mkdir -p "$APP_DIR/Contents/Frameworks"

    cp src/Platform/macOS/Deadly\ Stages\ Remix.icns "$APP_DIR/Contents/Resources/"
    cp src/Platform/macOS/Info.plist "$APP_DIR/Contents/"
    mv "$BASE_DIR/DeadlyStagesRemix" "$APP_DIR/Contents/MacOS"
    mv "$BASE_DIR/res" "$APP_DIR/Contents/Resources/"
    cp -r $HOME/sdl2-mac-frameworks/* "$APP_DIR/Contents/Frameworks/"

    (cd "$BASE_DIR" && zip -r "DeadlyStagesRemix-$DEADLY_STAGES_VERSION-macos-arm64-$BUILD_TYPE.zip" . -x "*.DS_Store")

    rm -rf "$APP_DIR"
fi
