cmake_minimum_required(VERSION 3.27)
project(XyrisTools LANGUAGES C CXX ASM_NASM)

list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../CMake/Modules")
include(InitializeStandardFlags)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_CURRENT_LIST_DIR}/Distribution"
        CACHE PATH "Install path prefix, prepended onto install directories." FORCE
    )
endif()

set(MODULES_KERNEL_DIR ${PROJECT_SOURCE_DIR}/../Modules/Kernel)

message(STATUS "[*] Tools")
add_subdirectory(VerifyGDT)
