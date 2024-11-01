#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/parse-build-type.sh"
source "$(dirname "${BASH_SOURCE[0]}")/pre-run.sh"

FINAL_RES_DIR="build/psp-$BUILD_TYPE/res"

mkdir -p $FINAL_RES_DIR
rsync -av --exclude '*.tiled-*' \
    --exclude '.DS_Store' \
    --exclude 'level/*/preview_*.png' \
    --exclude 'music/*' \
    --exclude 'level/*/music*' \
    res/ $FINAL_RES_DIR

# Level Previews
for dir in res/level/*; do
    if [ -d "$dir" ]; then
        level_name=$(basename "$dir")
        if [ -f "$dir/preview_128.png" ]; then
            cp "$dir/preview_128.png" "$FINAL_RES_DIR/level/$level_name/preview.png"
        fi
    fi
done

# Music
for file in res/music/*_psp.wav; do
    cp "$file" "$FINAL_RES_DIR/music/$(basename "$file" | sed 's/_psp//')"
done
for dir in res/level/*; do
    if [ -d "$dir" ]; then
        level_name=$(basename "$dir")
        if [ -f "$dir/music_psp.wav" ]; then
            cp "$dir/music_psp.wav" "$FINAL_RES_DIR/level/$level_name/music.wav"
        fi
    fi
done

