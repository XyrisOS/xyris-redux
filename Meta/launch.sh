#!/usr/bin/env bash
# shellcheck disable=SC2154
# shellcheck disable=SC2034
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)
source "$SCRIPT_DIR/common.sh"

do_run_stage_debug=0
do_run_stage_background=0
action="run"

show_help() {
    cat <<EOF
Usage: ./Meta/launch.sh [options]

Options:
  -d, --debug               Run Xyris in QEMU with the GDB stub enabled
  -h, --help                Show this help text
  -r, --run                 Run Xyris in QEMU
      --release             Use the Release build type
      --build-type <type>   Use a specific CMake build type (default: Debug)
EOF
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
        -vga std
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

    if [ $do_run_stage_background = 1 ]; then
        echo -e "${light_blue}Starting qemu... (daemonized)${no_color}"
        arguments+=(
            -serial vc
            -daemonize
        )
    else
        echo -e "${light_blue}Starting qemu...${no_color}"
        arguments+=(
            -serial stdio
        )
    fi

    qemu-system-x86_64 "${arguments[@]}"
}

while (($#)); do
    case $1 in
        -h|-\?|--help)
            show_help
            exit
            ;;
        -d|--debug)
            action="run"
            do_run_stage_debug=1
            ;;
        -r|--run)
            action="run"
            ;;
        --release)
            build_type="Release"
            ;;
        --background)
            do_run_stage_background=1
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
    run|debug)
        run_stage
        ;;
esac
