FROM alpine:edge
LABEL org.opencontainers.image.description "Xyris build environment container"

RUN apk update

# Packages necessary to build the cross compiler
ARG CROSS_COMPILER_PKGS="wget bison flex mpc1-dev gmp-dev mpfr-dev texinfo build-base util-linux-dev"
RUN apk add --no-cache ${CROSS_COMPILER_PKGS}

# Environment variables for cross compiler
ENV BIN_VER="2.39"
ENV GCC_VER="12.2.0"
ENV CROSS_PREFIX="/opt/cross"
ENV CROSS_MAKEFLAGS="-j6"
ENV PATH="$CROSS_PREFIX/bin:$PATH"

# Build Binutils
WORKDIR /tmp
RUN export MAKEFLAGS="${CROSS_MAKEFLAGS}"; \
    wget "https://ftp.gnu.org/pub/gnu/binutils/binutils-${BIN_VER}.tar.gz"; \
    tar -xf "binutils-${BIN_VER}.tar.gz"; \
    for TARGET in "x86_64-elf"; do \
        echo "Building binutils for ${TARGET}"; \
        mkdir "build-binutils-${TARGET}"; \
        cd "build-binutils-${TARGET}"; \
        ../binutils-${BIN_VER}/configure --target="${TARGET}" --prefix="${CROSS_PREFIX}" --with-sysroot --disable-nls --disable-werror; \
        make; \
        make install-strip; \
        cd /tmp; \
        rm -r "build-binutils-${TARGET}"; \
    done; \
    rm "binutils-${BIN_VER}.tar.gz";

# Build GCC
WORKDIR /tmp
RUN export MAKEFLAGS="${CROSS_MAKEFLAGS}"; \
    wget "https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz"; \
    tar -xf "gcc-${GCC_VER}.tar.gz"; \
    for TARGET in "x86_64-elf"; do \
        echo "Building binutils for ${TARGET}"; \
        mkdir "build-gcc-${TARGET}"; \
        cd "build-gcc-${TARGET}"; \
        ../gcc-${GCC_VER}/configure --target="${TARGET}" --prefix="${CROSS_PREFIX}" --disable-nls --enable-languages=c,c++ --without-headers; \
        make all-gcc; \
        make all-target-libgcc; \
        make install-strip-gcc; \
        make install-strip-target-libgcc; \
        cd /tmp; \
        rm -r "build-gcc-${TARGET}"; \
    done; \
    rm "gcc-${GCC_VER}.tar.gz";

ARG BUILD_PKGS="cmake ninja nasm mold"
ARG UTIL_PKGS="git gdb-multiarch"
ARG DOCS_PKGS="doxygen graphviz jq"

RUN apk add --no-cache ${BUILD_PKGS}
RUN apk add --no-cache ${UTIL_PKGS}
RUN apk add --no-cache ${DOCS_PKGS}
