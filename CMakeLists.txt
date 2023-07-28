cmake_minimum_required(VERSION 3.27)
project(Xyris LANGUAGES C CXX ASM_NASM)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules)

# Kernel & Modules
add_subdirectory(Modules)
add_subdirectory(Tools)
