project(Kernel)
message(STATUS "  [-] Kernel")

add_subdirectory(Arch)
add_subdirectory(Swift)

add_executable(Kernel
    Entry.cpp
)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Kernel.ld)
set_target_properties(Kernel PROPERTIES
    LINK_DEPENDS ${LINKER_SCRIPT}
    # Force the kernel to link the Swift, C, and C++ code using the C++ linker
    LINKER_LANGUAGE CXX
)
target_link_options(Kernel PRIVATE -Wl,--script=${LINKER_SCRIPT})
target_link_libraries(Kernel PRIVATE
    Loader
    Arch
    Runtime
    KernelSwift
)

install(TARGETS Kernel DESTINATION ${CMAKE_INSTALL_PREFIX})
