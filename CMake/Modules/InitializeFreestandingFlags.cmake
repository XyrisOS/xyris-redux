set(LINKER_FREESTANDING_FLAGS
    "-nostdlib"
    "-lgcc"
)

set(C_CXX_FREESTANDING_FLAGS
    "-nodefaultlibs"
    "-ffreestanding"
    "-fstack-protector"
    "-fno-omit-frame-pointer"
)

set(CMAKE_CXX_FLAGS
    ${C_CXX_FREESTANDING_FLAGS}
    "-fno-use-cxa-atexit"
    "-fno-exceptions"
    "-fno-rtti"
)

set(CMAKE_C_FLAGS
    ${C_CXX_FREESTANDING_FLAGS}
    "-ffreestanding"
)

set(CMAKE_EXE_LINKER_FLAGS ${LINKER_FREESTANDING_FLAGS})
set(CMAKE_MODULE_LINKER_FLAGS ${LINKER_FREESTANDING_FLAGS})
set(CMAKE_STATIC_LINKER_FLAGS ${LINKER_FREESTANDING_FLAGS})
