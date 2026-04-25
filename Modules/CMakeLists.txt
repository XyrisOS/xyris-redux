cmake_minimum_required(VERSION 3.27)
project(Xyris LANGUAGES C CXX ASM_NASM)

list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules")
include(InitializeStandardFlags)
include(InitializeFreestandingFlags)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_CURRENT_SOURCE_DIR}/Distribution"
        CACHE PATH "Install path prefix, prepended onto install directories." FORCE
    )
endif()

message(STATUS "[*] Modules")
add_subdirectory(Loader)
add_subdirectory(Kernel)
add_subdirectory(Tools)

add_custom_target(Images
    COMMAND "${CMAKE_COMMAND}" --install "${CMAKE_BINARY_DIR}" --prefix "${CMAKE_INSTALL_PREFIX}"
    COMMAND "${CMAKE_COMMAND}"
        -DOUTPUT_DIR="${CMAKE_INSTALL_PREFIX}"
        -DKERNEL_BIN="${CMAKE_INSTALL_PREFIX}/Kernel"
        -DLIMINE_CFG="${CMAKE_INSTALL_PREFIX}/limine.cfg"
        -DLIMINE_EFI="${CMAKE_CURRENT_SOURCE_DIR}/Tools/limine/BOOTX64.EFI"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Scripts/CreateModuleImages.cmake"
    DEPENDS Kernel
    USES_TERMINAL
    VERBATIM
    COMMENT "Installing kernel artifacts and generating disk images"
)

add_custom_target(Distribution DEPENDS Images)
