#!/bin/bash

# This script is meant to be used only from another script. Don't use it directly.

set -e

if [ $# -eq 0 ]; then
    echo "Usage: $0 <debug|release>" 
    exit 1
fi

BUILD_TYPE="$1"
if [ "$BUILD_TYPE" != "debug" ] && [ "$BUILD_TYPE" != "release" ]; then
    echo "Invalid build type: $BUILD_TYPE"
    echo "Valid options are: debug, release"
    exit 1
fi
