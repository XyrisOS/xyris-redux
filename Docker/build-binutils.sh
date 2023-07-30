#!/bin/sh
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
BIN_VER="2.39"

. "${SCRIPT_DIR}/build-common.sh"

cd "${SCRIPT_DIR}" || exit
wget "https://ftp.gnu.org/pub/gnu/binutils/binutils-${BIN_VER}.tar.gz"
tar -xf "binutils-${BIN_VER}.tar.gz"
rm "binutils-${BIN_VER}.tar.gz"

for TARGET in "x86_64-elf"; do
    echo "Building binutils for ${TARGET}"
    mkdir "build-binutils-${TARGET}"
    cd "build-binutils-${TARGET}" || exit
    ../binutils-"${BIN_VER}"/configure --target="${TARGET}" --prefix="${CROSS_PREFIX}" --with-sysroot --disable-nls --disable-werror
    make
    make install-strip
    cd .. || exit
    rm -rf "build-binutils-${TARGET}"
done

rm -rf "binutils-${BIN_VER}"
