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
#include "Ports.hpp"

namespace PIC
{

// Types

enum ICW1Commands : uint8_t {
    ICW4_PRESENT    = 0x01, // ICW4 will be present
    SINGLE_MODE     = 0x02, // Single (cascade) mode
    INTERVAL_4      = 0x04, // Call address interval 4 (8)
    LEVEL_TRIGGERD  = 0x08, // Level triggered (edge) mode
    INITIALIZE      = 0x10, // Initialization
    FINALIZE        = 0xFF, // Finalize (disable) PIC
};

enum ICW4Commands : uint8_t {
    INTEL_8086_MODE = 0x01, // 8086 (not 8080) mode
    AUTO_MODE       = 0x02, // Auto (normal) EOI
    BUFFER_SLAVE    = 0x08, // Buffered mode/slave
    BUFFER_MASTER   = 0x0C, // Buffered mode/master
    SFN_MODE        = 0x10, // Special fully nested (not)
};

enum Addresses : uint8_t {
    PIC1            = 0x20, // IO base address for master PIC
    PIC2            = 0xA0, // IO base address for slave PIC
};

enum Operations : uint8_t {
    PIC1_COMMAND    = PIC1,
    PIC1_DATA       = (PIC1 + 1),
    PIC2_COMMAND    = PIC2,
    PIC2_DATA       = (PIC2 + 1),
};

enum Actions : uint8_t {
    END_INERRUPT    = 0x20,
    PIC2_CASCADE    = 0x02,
    PIC1_CASCADE    = 0x04,
};

// Functions

void Initialize(void)
{
    // TODO: There's a lot of magic going on here that I don't fully understand.

    // Start initialization of PIC
    // (in cascade mode so that slave PICs are also initialized)
    IO::WriteByte(PIC1_COMMAND, ICW1Commands::INITIALIZE | ICW1Commands::ICW4_PRESENT);
    IO::WriteByte(PIC2_COMMAND, ICW1Commands::INITIALIZE | ICW1Commands::ICW4_PRESENT);

    // Tell master that a slave PIC exists at IRQ2 (0000 0100)
    IO::WriteByte(PIC1_DATA, PIC1_CASCADE);
    // Tell slave that it should use a cascade identity of value `2`
    IO::WriteByte(PIC2_DATA, PIC2_CASCADE);

    // Update PICs to 8086 mode instead of 8080 mode
    IO::WriteByte(PIC1_DATA, ICW4Commands::INTEL_8086_MODE);
    IO::WriteByte(PIC2_DATA, ICW4Commands::INTEL_8086_MODE);
}

void Finalize(void)
{
    IO::WriteByte(PIC2_DATA, FINALIZE);
    IO::WriteByte(PIC1_DATA, FINALIZE);
}

void EndOfInterrupt(uint64_t id)
{
    // Interrupts and exceptions are treated the same and as such we need to
    // account for all the exceptions (32 of them) and then offset by the
    // interrupts that require acknowledgement on the secondary PIC (which is
    // 8 since the 8259 handles 8 interrupts per controller), which results
    // in a value of 0x28 (40 decimal).
    if (id >= 0x28) {
        IO::WriteByte(PIC2_COMMAND, END_INERRUPT);
    }

    IO::WriteByte(PIC1_COMMAND, END_INERRUPT);
}

}
