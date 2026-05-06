cmake_minimum_required(VERSION 3.27)
project(XyrisTools LANGUAGES C CXX ASM_NASM)

# Provide a reference to the top level project for shared sources
set(CMAKE_CURRENT_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_PROJECT_DIR}/CMake/Common)
include(InitializeStandardFlags)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(STATUS "[!] Install prefix: ${CMAKE_CURRENT_SOURCE_DIR}/Distribution")
    set_property(
            CACHE CMAKE_INSTALL_PREFIX
            PROPERTY VALUE ${CMAKE_CURRENT_SOURCE_DIR}/Distribution/${CMAKE_BUILD_TYPE}
    )
endif()

# Set the location of the kernel source files we'll be using for tools & testing
set(MODULES_KERNEL_DIR ${PROJECT_SOURCE_DIR}/../Modules/Kernel)

message(STATUS "[*] Tools")
add_subdirectory(VerifyGDT)
