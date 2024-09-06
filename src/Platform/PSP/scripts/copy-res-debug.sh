#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/pre-run.sh"

mkdir -p build/psp-debug/res
cp -rv res/* build/psp-debug/res
