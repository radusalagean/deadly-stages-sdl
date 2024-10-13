#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/pre-run.sh"

mkdir -p build/psp-debug/res
rsync -av --exclude '*.tiled-*' --exclude '.DS_Store' \
    --exclude='image/cursor.png' \
    res/ build/psp-debug/res
