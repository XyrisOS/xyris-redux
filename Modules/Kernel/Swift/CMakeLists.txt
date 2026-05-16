project(KernelSwift)
message(STATUS "    [-] KernelSwift")

add_library(KernelSwift OBJECT
    KernelSwift.swift
    include/SwiftBridge.h
    include/module.modulemap
)

target_include_directories(KernelSwift
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

