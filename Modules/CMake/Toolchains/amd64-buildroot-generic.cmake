# https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#cross-compiling-using-clang
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(XYRIS_CROSS_TRIPLE "x86_64-elf" CACHE STRING "GNU target triple for the cross toolchain")
set(XYRIS_CROSS_PREFIX "" CACHE PATH "Prefix containing bin/${XYRIS_CROSS_TRIPLE}-gcc and related tools")

if(NOT XYRIS_CROSS_PREFIX)
    get_filename_component(cross_prefix "${CMAKE_SOURCE_DIR}/../Toolchain/bin" ABSOLUTE)
    if(EXISTS "${cross_prefix}/bin/${XYRIS_CROSS_TRIPLE}-gcc")
        set(XYRIS_CROSS_PREFIX "${cross_prefix}" CACHE PATH "" FORCE)
    else()
        set(XYRIS_CROSS_PREFIX "/opt/cross" CACHE PATH "" FORCE)
    endif()
endif()

set(cross_bin "${XYRIS_CROSS_PREFIX}/bin")

# C / C++
set(CMAKE_C_COMPILER    "${cross_bin}/${XYRIS_CROSS_TRIPLE}-gcc")
set(CMAKE_CXX_COMPILER  "${cross_bin}/${XYRIS_CROSS_TRIPLE}-g++")
set(CMAKE_AR            "${cross_bin}/${XYRIS_CROSS_TRIPLE}-ar")
set(CMAKE_RANLIB        "${cross_bin}/${XYRIS_CROSS_TRIPLE}-ranlib")
set(CMAKE_NM            "${cross_bin}/${XYRIS_CROSS_TRIPLE}-nm")
set(CMAKE_OBJCOPY       "${cross_bin}/${XYRIS_CROSS_TRIPLE}-objcopy")
set(CMAKE_OBJDUMP       "${cross_bin}/${XYRIS_CROSS_TRIPLE}-objdump")
set(CMAKE_STRIP         "${cross_bin}/${XYRIS_CROSS_TRIPLE}-strip")

# Check if we can use `mold` since it's far more performant
find_program(MOLD_BIN_PATH mold)
if(MOLD_BIN_PATH)
    get_filename_component(MOLD_BIN_DIR "${MOLD_BIN_PATH}" DIRECTORY)
    # https://github.com/rui314/mold/issues/1032
    # These have to be added to compile options, NOT link options
    set(MOLD_FLAGS -B${MOLD_BIN_DIR} -fuse-ld=mold)
    # Ensure that these flags are not added to the nasm language
    # since it has no clue what these options are.
    add_compile_options(
        "$<$<COMPILE_LANGUAGE:CXX>:${MOLD_FLAGS}>"
        "$<$<COMPILE_LANGUAGE:C>:${MOLD_FLAGS}>"
    )
endif()

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
# Enable debug symbols for nasm source files
set(CMAKE_ASM_NASM_FLAGS_DEBUG "-g -Fdwarf")

# Compiler is freestanding and does not have crt0.o or libc, so the test will fail.
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
