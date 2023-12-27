cmake_minimum_required(VERSION 3.27)
message(STATUS "  [-] VerifyGDT")

add_executable(VerifyGDT
    main.cpp
    ${MODULES_KERNEL_DIR}/Arch/GDT.cpp
)

target_include_directories(VerifyGDT PRIVATE ${MODULES_KERNEL_DIR})

install(TARGETS VerifyGDT DESTINATION ${CMAKE_INSTALL_PREFIX})
