message(STATUS "      [-] amd64")

target_sources(Arch PRIVATE
    Arch.cpp
    CPU/GDT.asm
    CPU/GDT.cpp
    Interrupts/IDT.asm
    Interrupts/IDT.cpp
    Interrupts/Interrupts.cpp
    Interrupts/ISR.asm
    Devices/PIC.cpp
    IO/Ports.cpp
)

target_include_directories(Arch
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}
)
