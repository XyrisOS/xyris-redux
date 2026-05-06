#!/usr/bin/env bash
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")

export CROSS_PREFIX="${CROSS_PREFIX:=${SCRIPT_DIR}/bin}"

echo "Building toolchain and installing to '${CROSS_PREFIX}'"
echo "[*] binutils"
if ! eval "${SCRIPT_DIR}/build-binutils.sh"; then
  echo "[!] Failed to build binutils!"
  exit 1
fi
echo "[*] GCC"
if ! eval "${SCRIPT_DIR}/build-gcc.sh"; then
  echo "[!] Failed to build GCC!"
  exit 1
fi
echo "[*] GDB"
if ! eval "${SCRIPT_DIR}/build-gdb.sh"; then
  echo "[!] Failed to build GDB!"
  exit 1
fi
echo "Done!"
