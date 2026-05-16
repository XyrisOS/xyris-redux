message(STATUS "[*] Embedded Swift compiler: ${CMAKE_Swift_COMPILER}")
set(CMAKE_Swift_COMPILATION_MODE wholemodule)

set(SWIFT_EMBEDDED_FLAGS
    -parse-as-library
    -enable-experimental-feature Embedded
    -no-allocations
    -Osize
    -target ${XYRIS_SWIFT_TARGET_TRIPLE}
    -module-cache-path ${XYRIS_SWIFT_MODULE_CACHE_PATH}
)

add_compile_options(
    "$<$<COMPILE_LANGUAGE:Swift>:${SWIFT_EMBEDDED_FLAGS}>"
)
