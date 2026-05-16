cmake_minimum_required(VERSION 3.29)
cmake_policy(SET CMP0157 NEW)
project(Xyris LANGUAGES C CXX ASM_NASM Swift)

# Provide a reference to the top level project for shared sources
set(CMAKE_CURRENT_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules)
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_PROJECT_DIR}/CMake/Common)
include(InitializeStandardFlags)
include(InitializeFreestandingFlags)
include(InitializeEmbeddedSwift)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(STATUS "[!] Install prefix: ${CMAKE_CURRENT_SOURCE_DIR}/Distribution")
    set_property(
            CACHE CMAKE_INSTALL_PREFIX
            PROPERTY VALUE ${CMAKE_CURRENT_SOURCE_DIR}/Distribution/${CMAKE_BUILD_TYPE}
    )
endif()

message(STATUS "[*] Modules")
add_subdirectory(Loader)
add_subdirectory(Kernel)
add_subdirectory(Tools)

# Distribution (Final Target)
#   - EFI and Boot Image Creation
set(kernel_bin "${CMAKE_INSTALL_PREFIX}/Kernel")
set(limine_cfg "${CMAKE_INSTALL_PREFIX}/limine.cfg")
set(limine_efi "${CMAKE_CURRENT_SOURCE_DIR}/Tools/limine/BOOTX64.EFI")
set(boot_img "${CMAKE_INSTALL_PREFIX}/boot.img")
set(efi_img "${CMAKE_INSTALL_PREFIX}/efi.img")
set(create_kernel_images_script "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Scripts/CreateKernelImages.cmake")

add_custom_command(
    OUTPUT "${boot_img}" "${efi_img}"
    COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --prefix ${CMAKE_INSTALL_PREFIX}
    COMMAND ${CMAKE_COMMAND}
        -DBOOT_IMG=${boot_img}
        -DEFI_IMG=${efi_img}
        -DKERNEL_BIN=${kernel_bin}
        -DLIMINE_CFG=${limine_cfg}
        -DLIMINE_EFI=${limine_efi}
        -P ${create_kernel_images_script}
    DEPENDS
        Kernel
        "${CMAKE_CURRENT_SOURCE_DIR}/Loader/limine.cfg"
        "${limine_efi}"
        "${create_kernel_images_script}"
    USES_TERMINAL
    VERBATIM
    COMMENT "Installing kernel artifacts and generating disk images"
)

add_custom_target(KernelImages DEPENDS "${boot_img}" "${efi_img}")
add_custom_target(Distribution DEPENDS KernelImages)
