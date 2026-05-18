set(BUILD_SHARED_LIBS OFF)
set(LINKER_FREESTANDING_FLAGS -nostdlib -nostartfiles -static)
add_link_options(${LINKER_FREESTANDING_FLAGS})

# Unused for now since clang complains about them being unused
set(C_CXX_FREESTANDING_X86_64_FLAGS
    -mno-mmx
    -mno-sse
    -mno-sse2
)
# C / C++ flags
set(C_CXX_FREESTANDING_FLAGS
    -mcmodel=large
    -nodefaultlibs
    -nostdlib
    -ffreestanding
    -fstack-protector
    -fno-omit-frame-pointer
)
# C++ flags
set(CXX_FREESTANDING_FLAGS
    ${C_CXX_FREESTANDING_FLAGS}
    -mno-red-zone
    -fno-use-cxa-atexit
    -fno-exceptions
    -fno-rtti
)
# C flags
set(C_FREESTANDING_FLAGS
    ${C_CXX_FREESTANDING_FLAGS}
)
# Swift flags
# Enable "wmo" as needed by Embedded Swift
set(CMAKE_Swift_COMPILATION_MODE wholemodule)
set(SWIFT_FREESTANDING_FLAGS
    -target ${XYRIS_SWIFT_TARGET_TRIPLE}
    -Osize
    # TODO: Remove this once we have allocators (or stubs) in Loader and Kernel
    -no-allocations
)

add_compile_options(
    "$<$<COMPILE_LANGUAGE:CXX>:${CXX_FREESTANDING_FLAGS}>"
    "$<$<COMPILE_LANGUAGE:C>:${C_FREESTANDING_FLAGS}>"
    "$<$<COMPILE_LANGUAGE:Swift>:${SWIFT_FREESTANDING_FLAGS}>"
)
