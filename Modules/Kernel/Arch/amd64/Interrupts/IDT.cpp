/**
 * @file IDT.cpp
 * @author Keeton Feavel (kfeavel@xyr.is)
 * @brief
 * @version 0.1
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "IDT.hpp"
#include "CPU/GDT.hpp"
#include <stddef.h>

namespace Arch::IDT
{

// Variables

// Defined by ISR.asm
extern "C" void* InterruptTable[256];
static_assert(sizeof(InterruptTable) / sizeof(InterruptTable[0]) == sizeof(IDT) / sizeof(Entry), "Interrupt table and IDT sizes differ");

static auto idt = IDT();
static auto idtr = IDTR();

// Functions

// Implemented by IDT.asm
extern "C" void FlushIDT(IDTR* pIDTR);

static void commitAndFlush()
{
    // Update IDT register and flush
    idtr = {
        .size = sizeof(idt) - 1,
        .addr = idt.Address(),
    };

    FlushIDT(&idtr);
}

static void createEntry(
    Entry& entry,
    const Offset& offset,
    const Gate& type,
    const uint8_t stackTable = 0)
{
    constexpr unsigned int kernelCodeSelector = (GDT::Entries::KernelCodeIndex() * sizeof(GDT::Entry));

    entry = {
        .offsetLow = offset.section.low,
        .selector = kernelCodeSelector,
        .stackTable = stackTable,
        .reservedLow = 0,
        .type = type,
        .zero = 0,
        .privilege = 0,
        .present = 1,
        .offsetMid = offset.section.mid,
        .offsetHigh = offset.section.high,
        .reservedHigh = 0,
    };
}

void Initialize()
{
    constexpr size_t max = sizeof(InterruptTable) / sizeof(InterruptTable[0]);
    for (size_t i = 0; i < max; i++) {
        // Have the interrupt variable here for debugging atm.
        Offset offset = { .value = reinterpret_cast<uintptr_t>(InterruptTable[i]) };

        // Interrupt vector #8 is assigned to st
        const uint8_t stackTable = i == 8 ? 1 : 0;
        createEntry(idt.entries[i], offset, GateInterrupt, stackTable);
    }

    commitAndFlush();
}

}
