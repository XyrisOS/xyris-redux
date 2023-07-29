FROM alpine:edge
LABEL org.opencontainers.image.description "Xyris build environment container"

ARG BUILD_PKGS="cmake ninja clang mold nasm"
ARG UTIL_PKGS="e2fsprogs e2tools wget git gdb-multiarch"
ARG DOCS_PKGS="doxygen graphviz jq"
RUN apk update
# Install in 3 separate stages to prevent rebuilding certain stages
RUN apk add --no-cache ${BUILD_PKGS}
RUN apk add --no-cache ${UTIL_PKGS}
RUN apk add --no-cache ${DOCS_PKGS}
