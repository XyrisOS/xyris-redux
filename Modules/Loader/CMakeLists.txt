cmake_minimum_required(VERSION 3.27)
project(Loader)
message(STATUS "\t[-] Loader")

add_library(Loader STATIC
    Entry.cpp
)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Linker.ld)
