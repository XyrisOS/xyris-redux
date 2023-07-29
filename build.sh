#!/bin/sh
PROJECT_DIR=$(dirname "$(readlink -f "$0")")
DIR_BUILD="${PROJECT_DIR}/Build"
DIR_CMAKE="${PROJECT_DIR}/CMake"
DIR_TOOLCHAINS="${DIR_CMAKE}/Toolchains"

show_help() {
    echo "TODO"
}

check_requirements() {
    set -- "cmake" "ninja" "clang" "clang++" "mold"
    for program in "$@"; do
        if [ ! "$(command -v "${program}")" ]; then
            echo "Did not find '${program}' in \$PATH."
            exit 1
        fi
    done
}

clean_stage() {
    rm -r "${DIR_BUILD}"
}

build_stage() {
    if [ ! -e "${DIR_BUILD}" ]; then
        mkdir "${DIR_BUILD}"
    fi

    cmake \
        -B "${DIR_BUILD}" "${PROJECT_DIR}" \
        -G Ninja \
        -DCMAKE_TOOLCHAIN_FILE="${DIR_TOOLCHAINS}/amd64-buildroot-generic.cmake"

    cmake --build "${DIR_BUILD}"
}

while :; do
    case $1 in
        -h|-\?|--help)
            show_help
            exit
            ;;
        -c|--clean)
            clean_stage
            exit
            ;;
        --)
            # End of all options.
            shift
            break
            ;;
        -?*)
            echo "Unknown option (ignored): '$1'"
            ;;
        *)
            break
    esac

    shift
done

check_requirements
build_stage
