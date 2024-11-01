#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/parse-build-type.sh"
source "$(dirname "${BASH_SOURCE[0]}")/pre-run.sh"

rm -rvfd build/psp-$BUILD_TYPE