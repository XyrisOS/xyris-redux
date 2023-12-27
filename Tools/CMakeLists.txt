cmake_minimum_required(VERSION 3.27)
project(XyrisTools LANGUAGES C CXX ASM_NASM)

include(InitializeStandardFlags)

set(MODULES_KERNEL_DIR ${PROJECT_SOURCE_DIR}/../Modules/Kernel)

message(STATUS "[*] Tools")
add_subdirectory(VerifyGDT)
