#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/../../Common/scripts/parse-build-type.sh"
source "$(dirname "${BASH_SOURCE[0]}")/../../Common/scripts/pre-run.sh"

FINAL_RES_DIR="build/linux-x86_64-$BUILD_TYPE/res"

source "$(dirname "${BASH_SOURCE[0]}")/../../Common/scripts/copy-res-platform-group-computer.sh"