cmake_minimum_required(VERSION 3.27)
message(STATUS "  [-] VerifyGDT")

add_executable(VerifyGDT
    main.cpp
)

target_include_directories(VerifyGDT PRIVATE ${PROJECT_SOURCE_DIR}/Modules/Kernel)
