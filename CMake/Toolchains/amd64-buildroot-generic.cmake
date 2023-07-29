# https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#cross-compiling-using-clang
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(TRIPLE x86_64-elf)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET ${triple})

set(LINKER_BINARY lld)
# find_program(LINKER_EXECUTABLE mold)
# if (LINKER_EXECUTABLE)
#     set(LINKER_BINARY mold)
# endif()
# add_link_options(-fuse-ld=${LINKER_BINARY})
