#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/pre-run.sh"

mkdir -p build/psp-release/res
rsync -av --exclude '*.tiled-*' \
    --exclude '.DS_Store' \
    --exclude 'level/*/preview_*.png' \
    res/ build/psp-release/res

for dir in res/level/*; do
    if [ -d "$dir" ]; then
        level_name=$(basename "$dir")
        if [ -f "$dir/preview_128.png" ]; then
            cp "$dir/preview_128.png" "build/psp-release/res/level/$level_name/preview.png"
        fi
    fi
done