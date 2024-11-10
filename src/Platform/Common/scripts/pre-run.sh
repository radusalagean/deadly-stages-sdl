#!/bin/bash

# This script is meant to be used only from another script. Don't use it directly.

set -e

PROJECT_ROOT_DIR=$(dirname "${BASH_SOURCE[0]}")/../../../..
cd "$PROJECT_ROOT_DIR"

DEADLY_STAGES_VERSION="1.0.0"