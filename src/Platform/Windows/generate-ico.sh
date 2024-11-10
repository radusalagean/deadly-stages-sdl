#!/bin/bash

# Place your png icon in the same directory as this script, name it icon_32.png, then run this script

set -e

cd $(dirname "${BASH_SOURCE[0]}")

convert icon_32.png -define icon:auto-resize=32 icon_32.ico

x86_64-w64-mingw32-windres icon_32.rc -O coff -o icon_32.res