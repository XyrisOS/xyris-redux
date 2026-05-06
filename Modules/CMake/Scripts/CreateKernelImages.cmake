cmake_minimum_required(VERSION 3.27)

message(STATUS "[*] EFI / Boot Images")

foreach(name BOOT_IMG EFI_IMG KERNEL_BIN LIMINE_CFG LIMINE_EFI)
    if(NOT DEFINED ${name} OR "${${name}}" STREQUAL "")
        message(FATAL_ERROR "Missing required variable '${name}'.")
    endif()
endforeach()

foreach(path KERNEL_BIN LIMINE_CFG LIMINE_EFI)
    if(NOT EXISTS "${${path}}")
        message(FATAL_ERROR "Required input '${${path}}' does not exist.")
    endif()
endforeach()

foreach(program dd mformat mmd mcopy)
    find_program(${program}_bin NAMES "${program}" REQUIRED)
endforeach()

set(dd_block_size "1M")
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    set(dd_block_size "1m")
endif()

function(run_checked)
    execute_process(
        COMMAND ${ARGV}
        COMMAND_ECHO STDOUT
        COMMAND_ERROR_IS_FATAL ANY
    )
endfunction()

foreach(image BOOT_IMG EFI_IMG)
    get_filename_component(image_dir "${${image}}" DIRECTORY)
    file(MAKE_DIRECTORY "${image_dir}")
endforeach()

file(REMOVE "${EFI_IMG}" "${BOOT_IMG}")

message(STATUS "  [-] Creating ${EFI_IMG}")
run_checked("${dd_bin}" "if=/dev/zero" "of=${EFI_IMG}" "bs=${dd_block_size}" "count=32")
run_checked("${mformat_bin}" -i "${EFI_IMG}" -t 53 ::)
run_checked("${mmd_bin}" -i "${EFI_IMG}" ::/EFI)
run_checked("${mmd_bin}" -i "${EFI_IMG}" ::/EFI/BOOT)
run_checked("${mcopy_bin}" -i "${EFI_IMG}" "${LIMINE_EFI}" ::/EFI/BOOT)
run_checked("${mcopy_bin}" -i "${EFI_IMG}" "${LIMINE_CFG}" ::/EFI/BOOT)

message(STATUS "  [-] Creating ${BOOT_IMG}")
run_checked("${dd_bin}" "if=/dev/zero" "of=${BOOT_IMG}" "bs=${dd_block_size}" "count=32")
run_checked("${mformat_bin}" -i "${BOOT_IMG}" -t 53 ::)
run_checked("${mcopy_bin}" -i "${BOOT_IMG}" "${KERNEL_BIN}" ::)
