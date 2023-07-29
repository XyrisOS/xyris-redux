cmake_minimum_required(VERSION 3.27)
project(Xyris LANGUAGES C CXX ASM_NASM)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Kernel & Modules
add_subdirectory(Modules)
add_subdirectory(Tools)
