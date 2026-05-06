#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)
DIR_MODULES="${PROJECT_DIR}/Modules"
DIR_TOOLS="${PROJECT_DIR}/Tools"
MODULES_TOOLCHAIN_FILE="${DIR_MODULES}/CMake/Toolchains/amd64-buildroot-generic.cmake"

light_blue="\033[1;34m"
light_red="\033[1;31m"
yellow="\033[1;33m"
no_color="\033[0m"

build_type="${BUILD_TYPE:-Debug}"
do_build_images=1
do_run_stage_debug=0
action="build"

show_error() {
    echo -e "${light_red}$1${no_color}"
}

require_programs() {
    local program=""
    for program in "$@"; do
        if ! command -v "${program}" >/dev/null 2>&1; then
            show_error "Did not find '${program}' in \$PATH."
            exit 1
        fi
    done
}

normalize_build_type() {
    case "${build_type,,}" in
        debug)
            build_type="Debug"
            ;;
        release)
            build_type="Release"
            ;;
        relwithdebinfo)
            build_type="RelWithDebInfo"
            ;;
        minsizerel)
            build_type="MinSizeRel"
            ;;
    esac
}

show_help() {
    cat <<EOF
Usage: ./build.sh [options]

Options:
  -c, --clean               Remove all generated build and install directories
  -d, --debug               Run qemu with the GDB stub enabled
  -h, --help                Show this help text
      --image               Build EFI and boot images via the Modules Distribution target (default)
  -k, --kernel-only         Skip image generation and only build/install binaries
  -r, --run                 Run qemu using existing images for the selected build type
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

run_stage() {
    local modules_install_dir="${DIR_MODULES}/Distribution/${build_type}"
    local ovmf
    local efi
    local boot

    if ! command -v qemu-system-x86_64 >/dev/null 2>&1; then
        show_error "Did not find 'qemu-system-x86_64' in \$PATH."
        return
    fi

    ovmf=${OVMF:-"${PROJECT_DIR}/Tools/OVMF.bin"}
    efi="${modules_install_dir}/efi.img"
    boot="${modules_install_dir}/boot.img"

    if [ ! -e "${ovmf}" ]; then
        show_error "Did not find '${ovmf}'."
        return
    fi
    if [ ! -e "${efi}" ]; then
        show_error "Did not find '${efi}'."
        return
    fi
    if [ ! -e "${boot}" ]; then
        show_error "Did not find '${boot}'."
        return
    fi

    # shellcheck disable=SC2054
    local arguments=(
        -bios "${ovmf}"
        -device ide-cd,bus=ide.0,drive=efi,bootindex=0
        -drive "if=none,media=cdrom,id=efi,file=${efi}"
        -device ide-cd,bus=ide.1,drive=boot,bootindex=1
        -drive "if=none,media=cdrom,id=boot,file=${boot}"
        -m 4G
        -rtc clock=host
        -serial stdio
        -monitor telnet:127.0.0.1:4444,server,nowait
    )

    if [ $do_run_stage_debug = 1 ]; then
        echo
        echo -e "${light_blue}Attach to qemu with GDB${no_color}"
        echo
        echo -e "${yellow}set arch i386:x86-64:intel${no_color}"
        echo -e "${yellow}target remote localhost:1234${no_color}"
        echo
        arguments+=(-s -S)
    fi

    echo -e "${light_blue}Starting qemu...${no_color}"
    qemu-system-x86_64 "${arguments[@]}"
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
            action="run"
            do_run_stage_debug=1
            ;;
        --image)
            do_build_images=1
            ;;
        -k|--kernel-only)
            do_build_images=0
            ;;
        -r|--run)
            action="run"
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
    run)
        run_stage
        ;;
esac
