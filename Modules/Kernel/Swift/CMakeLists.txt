project(KernelSwift)
message(STATUS "    [-] KernelSwift")

add_library(KernelSwift OBJECT
    KernelSwift.swift
)
target_include_directories(KernelSwift PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(KernelSwiftTargetProbe OBJECT
    TargetProbe.swift
)
