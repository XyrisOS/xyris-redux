#!/usr/bin/env bash
# shellcheck disable=SC2154
# shellcheck disable=SC2034
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)
source "$SCRIPT_DIR/common.sh"

MODULES_TOOLCHAIN_FILE="${DIR_MODULES}/CMake/Toolchains/amd64-buildroot-generic.cmake"

do_build_images=1
action="build"

show_help() {
    cat <<EOF
Usage: ./Meta/build.sh [options]

Options:
  -c, --clean               Remove all generated build and install directories
  -d, --debug               Use the Debug build type
  -h, --help                Show this help text
      --image               Build EFI and boot images via the Modules Distribution target (default)
  -k, --kernel-only         Skip image generation and only build/install binaries
      --release             Use the Release build type
      --build-type <type>   Use a specific CMake build type (default: Debug)
EOF
}

configure_project() {
    local source_dir=$1
    local build_dir=$2
    local install_dir=$3
    shift 3

    mkdir -p "${build_dir}" "${install_dir}"

    cmake \
        -G Ninja \
        -S "${source_dir}" \
        -B "${build_dir}" \
        -DCMAKE_BUILD_TYPE="${build_type}" \
        -DCMAKE_INSTALL_PREFIX="${install_dir}" \
        "$@"
}

clean_stage() {
    local path=""
    local -a paths=()

    shopt -s nullglob
    paths=(
        "${PROJECT_DIR}/Build"
        "${PROJECT_DIR}/Distribution"
        "${DIR_TOOLS}/Build"
        "${DIR_TOOLS}/Distribution"
        "${DIR_MODULES}/Build"
        "${DIR_MODULES}/Distribution"
    )
    shopt -u nullglob

    echo -e "### ${light_blue}Cleaning generated output directories${no_color}"
    for path in "${paths[@]}"; do
        if [ -e "${path}" ]; then
            if [[ "${path}" != "${PROJECT_DIR}/"* ]]; then
                show_error "Refusing to remove path outside the project: '${path}'."
                exit 1
            fi
            echo -e "${yellow}Removing '${path}'...${no_color}"
            rm -rf "${path:?}"
        fi
    done
}

build_stage() {
    local build_dir_tools="${DIR_TOOLS}/Build/${build_type}"
    local install_dir_tools="${DIR_TOOLS}/Distribution/${build_type}"
    local build_dir_modules="${DIR_MODULES}/Build/${build_type}"
    local install_dir_modules="${DIR_MODULES}/Distribution/${build_type}"
    local modules_configure_args=()

    require_programs cmake ninja nasm

    if [ ! -e "${MODULES_TOOLCHAIN_FILE}" ]; then
        show_error "Did not find '${MODULES_TOOLCHAIN_FILE}'."
        exit 1
    fi

    echo -e "### ${light_blue}Configuring Tools (${build_type})${no_color}"
    configure_project "${DIR_TOOLS}" "${build_dir_tools}" "${install_dir_tools}"

    echo -e "### ${light_blue}Building Tools${no_color}"
    cmake --build "${build_dir_tools}" --parallel
    echo -e "### ${light_blue}Installing Tools${no_color}"
    cmake --install "${build_dir_tools}"

    echo -e "### ${light_blue}Configuring Kernel & Modules (${build_type})${no_color}"
    if [ ! -e "${build_dir_modules}/CMakeCache.txt" ]; then
        modules_configure_args=(-DCMAKE_TOOLCHAIN_FILE="${MODULES_TOOLCHAIN_FILE}")
    fi

    configure_project \
        "${DIR_MODULES}" \
        "${build_dir_modules}" \
        "${install_dir_modules}" \
        "${modules_configure_args[@]}"

    if [ "${do_build_images}" -eq 1 ]; then
        echo -e "### ${light_blue}Building Kernel, Modules, and Images${no_color}"
        cmake --build "${build_dir_modules}" --parallel --target Distribution
    else
        echo -e "### ${light_blue}Building Kernel & Modules${no_color}"
        cmake --build "${build_dir_modules}" --parallel
        echo -e "### ${light_blue}Installing Kernel & Modules${no_color}"
        cmake --install "${build_dir_modules}"
    fi
}

while (($#)); do
    case $1 in
        -c|--clean)
            action="clean"
            ;;
        -h|-\?|--help)
            show_help
            exit
            ;;
        -d|--debug)
            build_type="Debug"
            ;;
        -k|--kernel-only)
            do_build_images=0
            ;;
        --release)
            build_type="Release"
            ;;
        --build-type)
            shift
            if (($# == 0)); then
                show_help
                echo
                echo -e "${yellow}Missing value for '--build-type'${no_color}"
                exit 1
            fi
            build_type=$1
            ;;
        --)
            # End of all options.
            shift
            break
            ;;
        -?*)
            show_help
            echo
            echo -e "${yellow}Unknown option: '$1'${no_color}"
            exit
            ;;
        *)
            break
    esac

    shift
done

normalize_build_type

case "${action}" in
    build)
        build_stage
        ;;
    clean)
        clean_stage
        ;;
esac
