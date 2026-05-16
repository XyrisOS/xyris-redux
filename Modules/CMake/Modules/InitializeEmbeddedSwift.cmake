set(
    XYRIS_SWIFT_TARGET_TRIPLE
    "x86_64-unknown-none-elf"
    CACHE STRING
    "Embedded Swift target triple for kernel components"
)
set(
    XYRIS_SWIFT_TARGET_CPU
    ""
    CACHE STRING
    "Optional Embedded Swift target CPU for kernel components"
)
if(NOT CMAKE_Swift_COMPILER)
    message(FATAL_ERROR
        "Embedded Swift requires swiftc, but none was found. "
        "Install the pinned Swift toolchain or configure CMake with "
        "-DCMAKE_Swift_COMPILER=/absolute/path/to/swiftc."
    )
endif()

message(STATUS "[*] Embedded Swift compiler: ${CMAKE_Swift_COMPILER}")

function(xyris_configure_embedded_swift_target target_name)
    target_compile_options(${target_name} PRIVATE
        "$<$<COMPILE_LANGUAGE:Swift>:-parse-as-library>"
        "$<$<COMPILE_LANGUAGE:Swift>:-enable-experimental-feature>"
        "$<$<COMPILE_LANGUAGE:Swift>:Embedded>"
        "$<$<COMPILE_LANGUAGE:Swift>:-target>"
        "$<$<COMPILE_LANGUAGE:Swift>:${XYRIS_SWIFT_TARGET_TRIPLE}>"
        "$<$<COMPILE_LANGUAGE:Swift>:-module-cache-path>"
        "$<$<COMPILE_LANGUAGE:Swift>:${XYRIS_SWIFT_MODULE_CACHE_PATH}>"
    )

    if(XYRIS_SWIFT_TARGET_CPU)
        target_compile_options(${target_name} PRIVATE
            "$<$<COMPILE_LANGUAGE:Swift>:-target-cpu>"
            "$<$<COMPILE_LANGUAGE:Swift>:${XYRIS_SWIFT_TARGET_CPU}>"
        )
    endif()
endfunction()
