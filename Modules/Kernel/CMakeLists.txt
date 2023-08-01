cmake_minimum_required(VERSION 3.27)
project(Kernel)
message(STATUS "\t[-] Kernel")

add_executable(Kernel
    Entry.cpp
)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Linker.ld)
set_target_properties(Kernel PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
target_link_options(Kernel PRIVATE -Wl,--script=${LINKER_SCRIPT})
target_link_libraries(Kernel PRIVATE Loader)

install(TARGETS Kernel DESTINATION ${CMAKE_INSTALL_PREFIX})
