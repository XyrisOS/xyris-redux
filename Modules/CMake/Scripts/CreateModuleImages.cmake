cmake_minimum_required(VERSION 3.27)

foreach(required_var OUTPUT_DIR KERNEL_BIN LIMINE_CFG LIMINE_EFI)
    if (NOT DEFINED ${required_var} OR "${${required_var}}" STREQUAL "")
        message(FATAL_ERROR "Missing required variable '${required_var}'.")
    endif()
endforeach()

foreach(required_file KERNEL_BIN LIMINE_CFG LIMINE_EFI)
    if (NOT EXISTS "${${required_file}}")
        message(FATAL_ERROR "Required input '${${required_file}}' does not exist.")
    endif()
endforeach()

foreach(program dd mformat mmd mcopy)
    find_program(${program}_BIN NAMES "${program}")
    if (NOT DEFINED ${program}_BIN OR NOT "${${program}_BIN}")
        message(FATAL_ERROR "Did not find '${program}' in PATH.")
    endif()
endforeach()

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    set(_xyris_dd_block_size "1m")
else()
    set(_xyris_dd_block_size "1M")
endif()

function(_xyris_run_checked)
    execute_process(
        COMMAND ${ARGV}
        COMMAND_ECHO STDOUT
        RESULT_VARIABLE _xyris_result
    )
    if (NOT _xyris_result EQUAL 0)
        message(FATAL_ERROR "Command failed with exit code ${_xyris_result}.")
    endif()
endfunction()

file(MAKE_DIRECTORY "${OUTPUT_DIR}")

set(_xyris_efi_image "${OUTPUT_DIR}/efi.img")
set(_xyris_boot_image "${OUTPUT_DIR}/boot.img")

file(REMOVE "${_xyris_efi_image}" "${_xyris_boot_image}")

message(STATUS "Creating ${_xyris_efi_image}")
_xyris_run_checked("${dd_BIN}" "if=/dev/zero" "of=${_xyris_efi_image}" "bs=${_xyris_dd_block_size}" "count=32")
_xyris_run_checked("${mformat_BIN}" -i "${_xyris_efi_image}" -t 53 ::)
_xyris_run_checked("${mmd_BIN}" -i "${_xyris_efi_image}" ::/EFI)
_xyris_run_checked("${mmd_BIN}" -i "${_xyris_efi_image}" ::/EFI/BOOT)
_xyris_run_checked("${mcopy_BIN}" -i "${_xyris_efi_image}" "${LIMINE_EFI}" ::/EFI/BOOT)
_xyris_run_checked("${mcopy_BIN}" -i "${_xyris_efi_image}" "${LIMINE_CFG}" ::/EFI/BOOT)

message(STATUS "Creating ${_xyris_boot_image}")
_xyris_run_checked("${dd_BIN}" "if=/dev/zero" "of=${_xyris_boot_image}" "bs=${_xyris_dd_block_size}" "count=32")
_xyris_run_checked("${mformat_BIN}" -i "${_xyris_boot_image}" -t 53 ::)
_xyris_run_checked("${mcopy_BIN}" -i "${_xyris_boot_image}" "${KERNEL_BIN}" ::)
