cmake_minimum_required(VERSION 3.27)
project(Loader)
message(STATUS "    [-] Arch")

add_library(Arch OBJECT
    Arch.cpp
    GDT.asm
    GDT.cpp
    IDT.asm
    IDT.cpp
    Interrupts.cpp
    ISR.asm
    PIC.cpp
    Ports.cpp
)

# TODO: I have yet to decide if / how I want to go about public v. private headers
#       in the kernel. For the Loader it makes more sense to keep them separate since
#       the kernel and loader should be two distinct things that interact as little as
#       possible (and mostly unidirectionally), but internal kernel code is different.
target_include_directories(Arch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
