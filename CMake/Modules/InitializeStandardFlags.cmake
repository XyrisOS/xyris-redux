set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(C_WARNINGS
    "-Wnested-externs"
    "-Wstrict-prototypes"
    "-Wmissing-prototypes"
)

set(CXX_WARNINGS
    # Empty for now
)

# C & C++ common flags
set(C_CXX_WARNINGS
    "-Wall"
    "-Werror"
    "-Wextra"
    "-Wundef"
    "-Winline"
    "-Wshadow"
    "-Wformat=2"
    "-Wcast-align"
    "-Wno-long-long"
    "-Wpointer-arith"
    "-Wwrite-strings"
    "-Wredundant-decls"
    "-Wdouble-promotion"
    "-Wno-unused-function"
    "-Wmissing-declarations"
)

set(CMAKE_CXX_FLAGS
    ${C_CXX_WARNINGS}
)
set(CMAKE_C_FLAGS
    ${C_WARNINGS}
    ${C_CXX_WARNINGS}
)
