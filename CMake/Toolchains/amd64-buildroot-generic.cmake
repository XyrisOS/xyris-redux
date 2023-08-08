# https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#cross-compiling-using-clang
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(triple x86_64-elf)

set(CMAKE_C_COMPILER ${triple}-gcc)
set(CMAKE_CXX_COMPILER ${triple}-g++)

# Check if we can use `mold` since it's far more performant
find_program(MOLD_BIN_PATH mold)
if (MOLD_BIN_PATH)
    # https://github.com/rui314/mold/issues/1032
    # These have to be added to compile options, NOT link options
    set(MOLD_FLAGS -B${MOLD_BIN_PATH} -fuse-ld=mold)
    # Ensure that these flags are not added to the nasm language
    # since it has no clue what these options are.
    add_compile_options(
        "$<$<COMPILE_LANGUAGE:CXX>:${MOLD_FLAGS}>"
        "$<$<COMPILE_LANGUAGE:C>:${MOLD_FLAGS}>"
    )
endif()

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)

# Compiler is freestanding and does not have crt0.o or libc, so the test will fail.
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
