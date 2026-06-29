set(BUILD_SHARED_LIBS OFF)
set(LINKER_FREESTANDING_FLAGS -nostdlib -nostartfiles -static)
add_link_options(${LINKER_FREESTANDING_FLAGS})

# Keep kernel code on general-purpose registers until extended processor state
# is saved and restored across interrupts and context switches.
set(C_CXX_FREESTANDING_X86_64_FLAGS
    -mno-mmx
    -mno-sse
    -mno-sse2
    -mgeneral-regs-only
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
# AMD64 specific flags
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    set(C_FREESTANDING_FLAGS
        ${C_FREESTANDING_FLAGS}
        ${C_CXX_FREESTANDING_X86_64_FLAGS}
    )
    set(CXX_FREESTANDING_FLAGS
        ${CXX_FREESTANDING_FLAGS}
        ${C_CXX_FREESTANDING_X86_64_FLAGS}
    )
endif()

add_compile_options(
    "$<$<COMPILE_LANGUAGE:CXX>:${CXX_FREESTANDING_FLAGS}>"
    "$<$<COMPILE_LANGUAGE:C>:${C_FREESTANDING_FLAGS}>"
)
