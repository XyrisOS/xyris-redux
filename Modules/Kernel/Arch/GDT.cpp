/**
 * @file GDT.cpp
 * @author Keeton Feavel (kfeavel@xyr.is)
 * @brief
 * @version 0.1
 * @date 2023-08-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "GDT.hpp"
#include <stddef.h>

namespace GDT
{

// Variables

static GDT gdt = GDT();
static GDTR gdtr = GDTR();

// Functions

// Implemented by GDT.asm
extern "C" void FlushGDT(GDTR* pGDTR);

static void CommitAndFlush(void)
{
    // Update GDT register and flush
    gdtr = {
        .size = sizeof(gdt) - 1,
        .addr = gdt.address(),
    };

    // TODO: The `lgdt` instruction is triple fauling. Figure out why.
    FlushGDT(&gdtr);
}

static void CreateEntry(
    Entry &entry,
    const Base &base,
    const Limit &limit,
    const bool executable,
    const uint8_t privilege)
{
    entry = {
        .limitLow = limit.section.low,
        .baseLow = base.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = (unsigned int)(executable ? 1 : 0),
        .system = 1,
        .privilege = privilege,
        .present = 1,
        .limitHigh = limit.section.high,
        .reserved = 0,
        .longMode = 1,
        .size = 0,
        .granulatity = 1,
        .baseHigh = base.section.high,
    };
}

void Initialize(void)
{
    // Base and limit are the same for all entries on x86_64
    const union Base base = { .value = 0 };
    const union Limit limit = { .value = 0 };

    CreateEntry(gdt.kernelCode(), base, limit, true, 0);
    CreateEntry(gdt.kernelData(), base, limit, false, 0);
    CreateEntry(gdt.userCode(), base, limit, true, 3);
    CreateEntry(gdt.userData(), base, limit, false, 3);

    CommitAndFlush();
}

}
