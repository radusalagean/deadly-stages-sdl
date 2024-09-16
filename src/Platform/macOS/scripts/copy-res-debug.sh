#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/pre-run.sh"

mkdir -p build/macos-arm64-debug/res
rsync -av --exclude '*.tiled-*' --exclude '.DS_Store' res/ build/macos-arm64-debug/res/
