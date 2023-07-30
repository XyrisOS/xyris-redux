#!/bin/sh

nproc="$(nproc)"
echo "Using ${nproc} cores to build tools..."

export CROSS_PREFIX="/opt/cross"
export MAKEFLAGS="-j${nproc}"
export PATH="$CROSS_PREFIX/bin:$PATH"
