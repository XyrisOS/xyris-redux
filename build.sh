#!/bin/sh
PROJECT_DIR=$(dirname "$(readlink -f "$0")")
DIR_BUILD="${PROJECT_DIR}/Build"
DIR_CMAKE="${PROJECT_DIR}/CMake"
DIR_TOOLCHAINS="${DIR_CMAKE}/Toolchains"

# Script arguments
verbose=0

show_help() {
    echo "TODO"
}

check_requirements() {
    set -- "cmake" "ninja" "clang" "clang++"
    for program in "$@"; do
        if [ ! "$(command -v "${program}")" ]; then
            echo "Did not find '${program}' in PATH."
            exit 1
        fi
    done
}

build_stage() {
    if [ "$verbose" = 1 ]; then echo "Building..."; fi
    if [ ! -e "${DIR_BUILD}" ]; then
        if [ "$verbose" = 1 ]; then echo "Creating '${DIR_BUILD}' directory..."; fi
        mkdir "${DIR_BUILD}"
    fi

    cmake \
        -B "${DIR_BUILD}" "${PROJECT_DIR}" \
        -DCMAKE_TOOLCHAIN_FILE="${DIR_TOOLCHAINS}/amd64-buildroot-generic.cmake"

    cmake --build "${DIR_BUILD}"
}

check_requirements
build_stage

while :; do
    case $1 in
        -h|-\?|--help)
            show_help
            exit
            ;;
        -v|--verbose)
            # Each -v argument adds 1 to verbosity.
            verbose=$((verbose + 1))
            ;;
        -c|--clean)

            ;;
        --)
            # End of all options.
            shift
            break
            ;;
        -?*)
            printf 'WARN: Unknown option (ignored): %s\n' "$1" >&2
            ;;
        *)
            break
    esac

    shift
done
