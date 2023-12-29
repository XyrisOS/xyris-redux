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

enum Actions : uint8_t {
    PIC1_COMMAND    = PIC1,
    PIC1_DATA       = (PIC1 + 1),
    PIC2_COMMAND    = PIC2,
    PIC2_DATA       = (PIC2 + 1),
};

// Functions

void Initialize(void)
{
    // TODO: There's a lot of magic going on here that I don't fully understand.

    // Save masks for restoration after PIC initialization
    // uint8_t mask1 = IO::ReadByte(PIC1_DATA);
    // uint8_t mask2 = IO::ReadByte(PIC2_DATA);

    // Start initialization of PIC
    // (in cascade mode so that slave PICs are also initialized)
    IO::WriteByte(PIC1_COMMAND, ICW1Commands::INITIALIZE | ICW1Commands::ICW4_PRESENT);
    IO::WriteByte(PIC2_COMMAND, ICW1Commands::INITIALIZE | ICW1Commands::ICW4_PRESENT);

    // Write master and slave PIC vector offsets
    // TODO: How are these offsets determined? Are they correct for a 64-bit environment?
    IO::WriteByte(PIC1_DATA, 0x20);
    IO::WriteByte(PIC2_DATA, 0x28);

    // Tell master that a slave PIC exists at IRQ2 (0000 0100)
    IO::WriteByte(PIC1_DATA, 0x04);
    // Tell slave that it should use a cascade identity of value `2`
    IO::WriteByte(PIC2_DATA, 0x02);

    // Update PICs to 8086 mode instead of 8080 mode
    IO::WriteByte(PIC1_DATA, ICW4Commands::INTEL_8086_MODE);
    IO::WriteByte(PIC2_DATA, ICW4Commands::INTEL_8086_MODE);

    // Restore masks saved before initialization
    // IO::WriteByte(PIC1_DATA, mask1);
    // IO::WriteByte(PIC2_DATA, mask2);
}

void Finalize(void)
{
    IO::WriteByte(PIC2_DATA, FINALIZE);
    IO::WriteByte(PIC1_DATA, FINALIZE);
}

void EndOfInterrupt(uint64_t id)
{
    constexpr uint8_t eoi = 0x20;

    // Interrupts and exceptions are treated the same and as such we need to
    // account for all the exceptions (32 of them) and then offset by the
    // interrupts that require acknowledgement on the secondary PIC (which is
    // 8 since the 8259 handles 8 interrupts per controller), which results
    // in a value of 0x28 (40 decimal).
    if (id >= 0x28) {
        IO::WriteByte(PIC2_COMMAND, eoi);
    }

    IO::WriteByte(PIC1_COMMAND, eoi);
}

}
