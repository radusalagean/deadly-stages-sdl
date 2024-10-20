#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/pre-run.sh"

mkdir -p build/psp-release/res
rsync -av --exclude '*.tiled-*' --exclude '.DS_Store' \
    res/ build/psp-release/res
