cmake_minimum_required(VERSION 3.27)
project(Xyris LANGUAGES C CXX ASM_NASM)

include(InitializeStandardFlags)
include(InitializeFreestandingFlags)

message(STATUS "[*] Modules")
add_subdirectory(Loader)
add_subdirectory(Kernel)
add_subdirectory(Tools)
