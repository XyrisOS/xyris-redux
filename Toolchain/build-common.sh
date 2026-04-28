#!/bin/bash
set -euo pipefail

nproc="$(nproc)"
echo "Using ${nproc} cores to build tools..."

export CROSS_PREFIX="${CROSS_PREFIX:=/opt/cross}"
export MAKEFLAGS="-j${nproc}"
export PATH="$CROSS_PREFIX/bin:$PATH"

export CROSS_TARGETS=(
    "x86_64-elf"
)
