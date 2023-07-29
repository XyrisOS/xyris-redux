cmake_minimum_required(VERSION 3.27)
project(Loader)
message(STATUS "\t[-] Loader")

add_executable(Loader
    Entry.cpp
)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Linker.ld)
set_target_properties(Loader PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
target_link_options(Loader PRIVATE -Wl,--script=${LINKER_SCRIPT})
