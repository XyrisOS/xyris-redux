#!/usr/bin/env bash
# shellcheck disable=SC2034
set -euo pipefail

# Common paths
PROJECT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")"/../ && pwd -P)
DIR_MODULES="${PROJECT_DIR}/Modules"
DIR_TOOLS="${PROJECT_DIR}/Tools"

# Common output formatters
light_blue="\033[1;34m"
light_red="\033[1;31m"
yellow="\033[1;33m"
no_color="\033[0m"

# Common Variables
build_type="${BUILD_TYPE:-Debug}"

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
