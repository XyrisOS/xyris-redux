project(KernelSwift)
message(STATUS "    [-] KernelSwift")

add_library(KernelSwift OBJECT
    KernelSwift.swift
)
target_include_directories(KernelSwift PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
xyris_configure_embedded_swift_target(KernelSwift)

add_library(KernelSwiftTargetProbe OBJECT
    TargetProbe.swift
)
xyris_configure_embedded_swift_target(KernelSwiftTargetProbe)
