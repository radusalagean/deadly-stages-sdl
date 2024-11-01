#!/bin/bash

source "$(dirname "${BASH_SOURCE[0]}")/../../pre-run.sh"

cd build/psp-debug
pspsh -e reset
pspsh -e ./DeadlyStagesRemix.prx