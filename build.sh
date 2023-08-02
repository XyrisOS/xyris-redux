#!/usr/bin/env bash

PROJECT_DIR=$(dirname "$(readlink -f "$0")")
DIR_TOOLS="${PROJECT_DIR}/Tools"
DIR_BUILD="${PROJECT_DIR}/Build"
DIR_INSTALL="${PROJECT_DIR}/Distribution"
DIR_CMAKE="${PROJECT_DIR}/CMake"
DIR_TOOLCHAINS="${DIR_CMAKE}/Toolchains"

light_blue="\033[1;34m"
light_red="\033[1;31m"
yellow="\033[1;33m"
no_color="\033[0m"

do_image_stage=1

show_help() {
    echo -e "TODO"
}

clean_stage() {
    echo -e "### ${light_blue}Cleaning${no_color}"
    if [ -e "${DIR_BUILD}" ]; then
        rm -r "${DIR_BUILD}"
    fi
}

build_stage_check_requirements() {
    for program in "cmake" "ninja" "nasm" "mold"; do
        if [ ! "$(command -v "${program}")" ]; then
            echo -e "${light_red}Did not find '${program}' in \$PATH.${no_color}"
            exit 1
        fi
    done
}

build_stage() {
    build_stage_check_requirements
    echo -e "### ${light_blue}Building${no_color}"
    if [ ! -e "${DIR_BUILD}" ]; then
        mkdir "${DIR_BUILD}"
    fi
    if [ ! -e "${DIR_INSTALL}" ]; then
        mkdir "${DIR_INSTALL}"
    fi

    cmake \
        -G Ninja \
        -B "${DIR_BUILD}" "${PROJECT_DIR}" \
        -DCMAKE_INSTALL_PREFIX="${DIR_INSTALL}" \
        -DCMAKE_TOOLCHAIN_FILE="${DIR_TOOLCHAINS}/amd64-buildroot-generic.cmake"
    cmake --build "${DIR_BUILD}" -j "$(nproc)"
    cmake --install "${DIR_BUILD}"
}

image_stage_check_requirements() {
    for program in "dd" "mformat" "mmd" "mcopy"; do
        if [ ! "$(command -v "${program}")" ]; then
            echo -e "${light_red}Did not find '${program}' in \$PATH.${no_color}"
            exit 1
        fi
    done
}

image_stage() {
    image_stage_check_requirements
    echo -e "### ${light_blue}Creating image${no_color}"
    if [ ! -e "${DIR_INSTALL}" ]; then
        echo -e "${light_red}'${DIR_INSTALL}' does not exist!${no_color}"
        exit
    fi

    local kernel_bin="${DIR_INSTALL}/Kernel"
    local limine_cfg="${DIR_INSTALL}/limine.cfg"
    local limime_efi="${DIR_TOOLS}/limine/BOOTX64.EFI"
    if [ ! -e "${limime_efi}" ]; then
        # TODO: Add submodule warning / error message function
        echo -e "${light_red}'${limime_efi}' does not exist! (Update your submodules!)${no_color}"
        exit
    fi

    # Darwin uses BSD style `dd` so we need to adjust some flags
    local bs="1M"
    if [ "$(uname)" = "Darwin" ]; then
        bs="1m"
    fi

    # FIXME: Migrate to `mpartition`

    # ------- Limine UEFI -------

    # 100 Mb is the minimum recommended size by most UEFI compatible systems
    # Linux and Windows tend to be around 512 Mb whereas macOS is 200 Mb.
    # We have no need for all that space, so for now we break convention.
    # 53 cylinders ~= 32 Mb

    local image_efi="${DIR_INSTALL}/efi.img"
    if [ -e "${image_efi}" ]; then
        echo -e "${yellow}Removing old '${image_efi}'...${no_color}"
        rm "${image_efi:?}"
    fi

    dd if=/dev/zero of="${image_efi}" bs="${bs}" count=32
    echo "Formatting ${image_efi}..."
    mformat -i "${image_efi}" -t 53 ::
    mmd -i "${image_efi}" ::/EFI
    mmd -i "${image_efi}" ::/EFI/BOOT
    mcopy -i "${image_efi}" "${limime_efi}" ::/EFI/BOOT
    mcopy -i "${image_efi}" "${limine_cfg}" ::/EFI/BOOT

    # ------- Kernel -------

    local image_boot="${DIR_INSTALL}/boot.img"
    if [ -e "${image_boot}" ]; then
        echo -e "${yellow}Removing old '${image_boot}'...${no_color}"
        rm "${image_boot:?}"
    fi

    dd if=/dev/zero of="${image_boot}" bs="${bs}" count=32
    echo "Formatting ${image_boot}..."
    mformat -i "${image_boot}" -t 53 ::
    mcopy -i "${image_boot}" "${kernel_bin}" ::
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
        -k|--kernel-only)
            do_image_stage=0
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

build_stage
if [ $do_image_stage = 1 ]; then
    image_stage
fi
