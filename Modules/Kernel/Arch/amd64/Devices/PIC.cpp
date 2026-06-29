/**
 * @file PIC.cpp
 * @author Keeton Feavel (kfeavel@xyr.is)
 * @brief 8259 Programmable Interrupt Controller
 * @version 0.1
 * @date 2023-12-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "PIC.hpp"
#include "IO/Ports.hpp"

namespace Arch::PIC
{

// Types

enum Addresses : uint8_t {
    PIC1 = 0x20,    // IO base address for master PIC
    PIC2 = 0xA0,    // IO base address for slave PIC
};

enum Operations : uint8_t {
    PIC1_DATA = PIC1 + 1,
    PIC2_DATA = PIC2 + 1,
};

// Functions

void Initialize()
{
    // Legacy PIC is unused for interrupt delivery and is considered legacy hard.
    // Set every bit in both interrupt mask registers so that all PIC IRQ lines remain masked.
    IO::WriteByte(PIC2_DATA, 0xFF);
    IO::WriteByte(PIC1_DATA, 0xFF);
}

}
