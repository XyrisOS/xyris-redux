cmake_minimum_required(VERSION 3.27)
message(STATUS "  [-] VerifyGDT")

add_executable(VerifyGDT
    main.cpp
    ${MODULES_KERNEL_DIR}/Arch/amd64/CPU/GDT.cpp
)

target_include_directories(VerifyGDT PRIVATE ${MODULES_KERNEL_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS VerifyGDT DESTINATION .)
