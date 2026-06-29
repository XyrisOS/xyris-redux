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

// Table of ISR stub entry-point addresses defined by ISR.asm to reference when initializing
extern "C" void* InterruptHandlerStubTable[256];
static_assert(
    sizeof(InterruptHandlerStubTable) / sizeof(InterruptHandlerStubTable[0]) == sizeof(IDT) / sizeof(Entry),
    "Interrupt table and IDT sizes differ");

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
    constexpr size_t max = sizeof(InterruptHandlerStubTable) / sizeof(InterruptHandlerStubTable[0]);
    for (size_t i = 0; i < max; i++) {
        Offset offset = { .value = reinterpret_cast<uintptr_t>(InterruptHandlerStubTable[i]) };

        // Interrupt vector #8 is assigned stack table #1
        const uint8_t stackTable = i == 8 ? 1 : 0;
        createEntry(idt.entries[i], offset, GateInterrupt, stackTable);
    }

    commitAndFlush();
}

}
