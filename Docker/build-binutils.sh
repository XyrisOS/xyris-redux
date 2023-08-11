#!/bin/bash
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
BIN_VER="2.39"

. "${SCRIPT_DIR}/build-common.sh"

cleanup() {
    echo "Cleaning up..."
    cd "${SCRIPT_DIR}" || exit
    if [ -e "binutils-${BIN_VER}.tar.gz" ]; then
        echo "Removing tarball..."
        rm "binutils-${BIN_VER}.tar.gz"
    fi
    if [ -e "binutils-${BIN_VER}" ]; then
        echo "Removing tarball extract..."
        rm -rf "binutils-${BIN_VER}"
    fi
    for TARGET in "${CROSS_TARGETS[@]}"; do
        if [ -e "build-binutils-${TARGET}" ]; then
            echo "Removing build directory..."
            rm -rf "build-binutils-${TARGET}"
        fi
    done
}

trap cleanup EXIT

cd "${SCRIPT_DIR}" || exit
wget "https://ftp.gnu.org/pub/gnu/binutils/binutils-${BIN_VER}.tar.gz"
tar -xf "binutils-${BIN_VER}.tar.gz"
if [ ! -e "binutils-${BIN_VER}.tar.gz" ]; then
    echo "Failed to download tarball"
    exit 1
fi

rm "binutils-${BIN_VER}.tar.gz"

for TARGET in "${CROSS_TARGETS[@]}"; do
    echo "Building binutils for ${TARGET}"
    mkdir "build-binutils-${TARGET}"
    cd "build-binutils-${TARGET}" || exit
    ../binutils-"${BIN_VER}"/configure \
        --target="${TARGET}" \
        --prefix="${CROSS_PREFIX}" \
        --with-sysroot \
        --disable-nls \
        --disable-werror
    make
    make install-strip
    cd .. || exit
    rm -rf "build-binutils-${TARGET}"
done

rm -rf "binutils-${BIN_VER}"
