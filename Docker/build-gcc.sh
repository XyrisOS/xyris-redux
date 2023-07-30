#!/bin/sh
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
GCC_VER="12.2.0"

. "${SCRIPT_DIR}/build-common.sh"

cd "${SCRIPT_DIR}" || exit
wget "https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz"
tar -xf "gcc-${GCC_VER}.tar.gz"
rm "gcc-${GCC_VER}.tar.gz"

for TARGET in "x86_64-elf"; do \
    echo "Building binutils for ${TARGET}"
    mkdir "build-gcc-${TARGET}"
    cd "build-gcc-${TARGET}" || exit
    ../gcc-"${GCC_VER}"/configure --target="${TARGET}" --prefix="${CROSS_PREFIX}" --disable-nls --enable-languages=c,c++ --without-headers
    make all-gcc
    make all-target-libgcc
    make install-strip-gcc
    make install-strip-target-libgcc
    cd .. || exit
    rm -r "build-gcc-${TARGET}"
done
