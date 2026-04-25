# https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#cross-compiling-using-clang
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(XYRIS_CROSS_TRIPLE "x86_64-elf" CACHE STRING "GNU target triple for the cross toolchain")
set(XYRIS_CROSS_PREFIX "" CACHE PATH "Prefix containing bin/${XYRIS_CROSS_TRIPLE}-gcc and related tools")

if (NOT XYRIS_CROSS_PREFIX)
    set(_xyris_repo_cross_prefix "${CMAKE_CURRENT_LIST_DIR}/../../Local/cross")
    if (EXISTS "${_xyris_repo_cross_prefix}/bin/${XYRIS_CROSS_TRIPLE}-gcc")
        set(XYRIS_CROSS_PREFIX "${_xyris_repo_cross_prefix}" CACHE PATH "" FORCE)
    elseif (EXISTS "/opt/cross/bin/${XYRIS_CROSS_TRIPLE}-gcc")
        set(XYRIS_CROSS_PREFIX "/opt/cross" CACHE PATH "" FORCE)
    else()
        set(XYRIS_CROSS_PREFIX "/opt/cross" CACHE PATH "" FORCE)
    endif()
endif()

set(_xyris_cross_bin_dir "${XYRIS_CROSS_PREFIX}/bin")

function(_xyris_require_cross_tool out_var tool_suffix)
    find_program(_tool_path
        NAMES "${XYRIS_CROSS_TRIPLE}-${tool_suffix}"
        PATHS "${_xyris_cross_bin_dir}"
        NO_DEFAULT_PATH
    )
    if (NOT _tool_path)
        message(FATAL_ERROR
            "Did not find '${XYRIS_CROSS_TRIPLE}-${tool_suffix}' under '${_xyris_cross_bin_dir}'. "
            "Set XYRIS_CROSS_PREFIX to the toolchain install prefix."
        )
    endif()
    set("${out_var}" "${_tool_path}" PARENT_SCOPE)
endfunction()

_xyris_require_cross_tool(CMAKE_C_COMPILER gcc)
_xyris_require_cross_tool(CMAKE_CXX_COMPILER g++)
_xyris_require_cross_tool(CMAKE_AR ar)
_xyris_require_cross_tool(CMAKE_RANLIB ranlib)
_xyris_require_cross_tool(CMAKE_NM nm)
_xyris_require_cross_tool(CMAKE_OBJCOPY objcopy)
_xyris_require_cross_tool(CMAKE_OBJDUMP objdump)
_xyris_require_cross_tool(CMAKE_STRIP strip)

# Check if we can use `mold` since it's far more performant
find_program(MOLD_BIN_PATH mold)
if (MOLD_BIN_PATH)
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
