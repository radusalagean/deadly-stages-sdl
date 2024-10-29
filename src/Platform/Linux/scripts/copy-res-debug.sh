#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/pre-run.sh"

mkdir -p build/linux-x86_64-debug/res
rsync -av --exclude '*.tiled-*' \
    --exclude '.DS_Store' \
    --exclude 'level/*/preview_*.png' \
    res/ build/linux-x86_64-debug/res

for dir in res/level/*; do
    if [ -d "$dir" ]; then
        level_name=$(basename "$dir")
        if [ -f "$dir/preview_512.png" ]; then
            cp "$dir/preview_512.png" "build/linux-x86_64-debug/res/level/$level_name/preview.png"
        fi
    fi
done