#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR=$(dirname "$(readlink -f "$0")")
DIR_INSTALL_MODULES="${PROJECT_DIR}/Modules/Distribution"

light_blue="\033[1;34m"
light_red="\033[1;31m"
yellow="\033[1;33m"
no_color="\033[0m"

do_run_stage_debug=0

show_help() {
    cat <<EOF
Usage: ./build.sh [options]

Options:
  -d, --debug         Run qemu with the GDB stub enabled
  -h, --help          Show this help text
  -r, --run           Run qemu using existing Distribution/Modules images
EOF
}

run_stage() {
    if [ ! "$(command -v qemu-system-x86_64)" ]; then
        echo -e "${light_red}Did not find 'qemu-system-x86_64' in \$PATH.${no_color}"
        return
    fi

    local ovmf=${OVMF:=Tools/OVMF.bin}
    local efi="${DIR_INSTALL_MODULES}/efi.img"
    local boot="${DIR_INSTALL_MODULES}/boot.img"

    if [ ! -e "${ovmf}" ]; then
        echo -e "${light_red}Did not find '${ovmf}'.${no_color}"
        return
    fi
    if [ ! -e "${efi}" ]; then
        echo -e "${light_red}Did not find '${efi}'.${no_color}"
        return
    fi
    if [ ! -e "${boot}" ]; then
        echo -e "${light_red}Did not find '${boot}'.${no_color}"
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
        -h|-\?|--help)
            show_help
            exit
            ;;
        -d|--debug)
            do_run_stage_debug=1
            run_stage
            exit
            ;;
        -r|--run)
            run_stage
            exit
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
