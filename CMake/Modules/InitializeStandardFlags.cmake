set(C_CXX_WARNINGS -Wall -Werror -Wextra -Wundef -Winline -Wshadow -Wformat=2 -Wcast-align -Wno-long-long -Wpointer-arith -Wwrite-strings -Wredundant-decls -Wdouble-promotion -Wno-unused-function -Wmissing-declarations)
set(C_WARNINGS ${C_CXX_WARNINGS} -Wnested-externs -Wstrict-prototypes -Wmissing-prototypes)
set(CXX_WARNINGS ${C_CXX_WARNINGS})

set(C_CXX_FLAGS -fdiagnostics-color=always)
set(C_FLAGS ${C_CXX_FLAGS} ${C_WARNINGS})
set(CXX_FLAGS ${C_CXX_FLAGS} ${CXX_WARNINGS})

add_compile_options(
    "$<$<COMPILE_LANGUAGE:CXX>:${CXX_FLAGS}>"
    "$<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>"
)

# Require certain language versions across tools and kernel
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

set(CMAKE_STATIC_LIBRARY_SUFFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
