FROM alpine:edge
LABEL org.opencontainers.image.description "Xyris build environment container"

RUN apk update

# Packages necessary to build the cross compiler
ARG CROSS_COMPILER_PKGS="bash wget bison flex mpc1-dev gmp-dev mpfr-dev texinfo build-base util-linux-dev"
RUN apk add --no-cache ${CROSS_COMPILER_PKGS}

COPY build-common.sh /tmp/build-common.sh
COPY build-binutils.sh /tmp/build-binutils.sh
COPY build-gcc.sh /tmp/build-gcc.sh
COPY build-gdb.sh /tmp/build-gdb.sh
RUN /tmp/build-binutils.sh
RUN /tmp/build-gcc.sh
RUN /tmp/build-gdb.sh

ARG BUILD_PKGS="cmake ninja nasm mold"
ARG UTIL_PKGS="git gdb-multiarch mtools"
ARG DOCS_PKGS="doxygen graphviz jq"

RUN apk add --no-cache ${BUILD_PKGS}
RUN apk add --no-cache ${UTIL_PKGS}
RUN apk add --no-cache ${DOCS_PKGS}

ENV PATH="/opt/cross/bin:$PATH"
