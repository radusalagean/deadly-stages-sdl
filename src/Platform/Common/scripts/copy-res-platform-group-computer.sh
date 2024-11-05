#!/bin/bash

# This script is meant to be used only from another script. Don't use it directly.

set -e

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
        if [ -f "$dir/preview_512.png" ]; then
            cp "$dir/preview_512.png" "$FINAL_RES_DIR/level/$level_name/preview.png"
        fi
    fi
done

# Music
for file in res/music/*_pc.ogg; do
    cp "$file" "$FINAL_RES_DIR/music/$(basename "$file" | sed 's/_pc//')"
done
for dir in res/level/*; do
    if [ -d "$dir" ]; then
        level_name=$(basename "$dir")
        if [ -f "$dir/music_pc.ogg" ]; then
            cp "$dir/music_pc.ogg" "$FINAL_RES_DIR/level/$level_name/music.ogg"
        fi
    fi
done