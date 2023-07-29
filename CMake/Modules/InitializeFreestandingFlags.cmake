set(BUILD_SHARED_LIBS OFF)
set(LINKER_FREESTANDING_FLAGS -nostdlib -nostartfiles -static)
add_link_options(${LINKER_FREESTANDING_FLAGS})

# Unused for now since clang complains about them being unused
set(C_CXX_FREESTANDING_X86_64_FLAGS -mno-mmx -mno-sse -mno-sse2)

set(C_CXX_FREESTANDING_FLAGS -mcmodel=large -nodefaultlibs -nostdlib -ffreestanding -fstack-protector -fno-omit-frame-pointer)
set(CXX_FLAGS ${C_CXX_FREESTANDING_FLAGS} -mno-red-zone -fno-use-cxa-atexit -fno-exceptions -fno-rtti)
set(C_FLAGS ${C_CXX_FREESTANDING_FLAGS})
add_compile_options(
    "$<$<COMPILE_LANGUAGE:CXX>:${CXX_FLAGS}>"
    "$<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>"
)
