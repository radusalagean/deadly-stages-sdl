#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/pre-run.sh"

mkdir -p build/linux-x86_64-debug/res
cp -rv res/* build/linux-x86_64-debug/res
