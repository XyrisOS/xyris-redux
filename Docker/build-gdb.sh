#!/bin/bash
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
GDB_VER="13.2"

. "${SCRIPT_DIR}/build-common.sh"

cleanup() {
    echo "Cleaning up..."
    cd "${SCRIPT_DIR}" || exit
    if [ -e "gdb-${GDB_VER}.tar.gz" ]; then
        echo "Removing tarball..."
        rm "gdb-${GDB_VER}.tar.gz"
    fi
    if [ -e "gdb-${GDB_VER}" ]; then
        echo "Removing tarball extract..."
        rm -rf "gdb-${GDB_VER}"
    fi
    for TARGET in "${CROSS_TARGETS[@]}"; do
        if [ -e "build-gdb-${TARGET}" ]; then
            echo "Removing build directory..."
            rm -rf "build-gdb-${TARGET}"
        fi
    done
}

trap cleanup EXIT

cd "${SCRIPT_DIR}" || exit
wget "http://ftp.gnu.org/gnu/gdb/gdb-${GDB_VER}.tar.gz"
tar -xf "gdb-${GDB_VER}.tar.gz"
if [ ! -e "gdb-${GDB_VER}.tar.gz" ]; then
    echo "Failed to download tarball"
    exit 1
fi

rm "gdb-${GDB_VER}.tar.gz"

DARWIN_FLAGS=()
if [ "$(uname)" = "Darwin" ]; then
    echo "Building on Darwin -- adding extra flags..."
    DARWIN_FLAGS+=(
        --with-libgmp-prefix=/opt/homebrew/Cellar/gmp/6.2.1_1
        --with-python=/opt/homebrew/bin/python3
    )
fi

for TARGET in "${CROSS_TARGETS[@]}"; do
    echo "Building GDB for ${TARGET}"
    mkdir "build-gdb-${TARGET}"
    cd "build-gdb-${TARGET}" || exit
    ../gdb-"${GDB_VER}"/configure \
        --target="${TARGET}" \
        --prefix="${CROSS_PREFIX}" \
        --enable-interwork \
        --enable-multilib \
        "${DARWIN_FLAGS[@]}"
    make
    make install
    cd .. || exit
    rm -rf "build-gdb-${TARGET}"
done

rm -rf "gdb-${GDB_VER}"
