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
#include "GDT.hpp"
#include <stddef.h>

namespace IDT
{

// Variables

// Defined by ISR.asm
extern "C" void* InterruptTable[256];
static_assert((sizeof(InterruptTable) / sizeof(InterruptTable[0])) == (sizeof(IDT) / sizeof(Entry)), "Interrupt table and IDT sizes differ");

static IDT idt = IDT();
static IDTR idtr = IDTR();

// Functions

// Implemented by IDT.asm
extern "C" void FlushIDT(IDTR* pIDTR);

static void CommitAndFlush(void)
{
    // Update IDT register and flush
    idtr = {
        .size = sizeof(idt) - 1,
        .addr = idt.address(),
    };

    FlushIDT(&idtr);
}

static void CreateEntry(
    Entry &entry,
    const Offset &offset,
    const Gate &type)
{
    constexpr unsigned int kernelCodeSelector = (GDT::GDT::kernelCodeIndex() * sizeof(GDT::Entry));

    entry = {
        .offsetLow = offset.section.low,
        .selector = kernelCodeSelector,
        .stackTable = 0,
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

void Initialize(void)
{
    for (size_t i = 0; i < (sizeof(InterruptTable) / sizeof(InterruptTable[i])); i++) {
        // Have the interrupt variable here for debugging atm.
        // TODO: Simplify by removing variable.
        void* interrupt = &InterruptTable[i];
        union Offset offset = { .value = reinterpret_cast<uintptr_t>(interrupt) };

        CreateEntry(idt.entries[i], offset, GateInterrupt);
    }

    CommitAndFlush();
}

}
