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

namespace GDT
{

// Variables

static auto gdt = GDT();
static auto gdtr = GDTR();

// Functions

// Implemented by GDT.asm
extern "C" void FlushGDT(GDTR* pGDTR);

static void commitAndFlush()
{
    // Update GDT register and flush
    gdtr = {
        .size = sizeof(gdt) - 1,
        .addr = gdt.Address(),
    };

    FlushGDT(&gdtr);
}

static void createEntry(
    Entry& entry,
    const Base& base,
    const Limit& limit,
    const bool executable,
    const uint8_t privilege)
{
    entry = {
        .limitLow = limit.section.low,
        .baseLow = base.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = static_cast<unsigned int>(executable ? 1 : 0),
        .system = 1,
        .privilege = privilege,
        .present = 1,
        .limitHigh = limit.section.high,
        .reserved = 0,
        .longMode = static_cast<unsigned int>(executable ? 1 : 0),
        .size = 0,
        .granularity = 1,
        .baseHigh = base.section.high,
    };
}

void Initialize()
{
    // Base and limit are the same for all entries on x86_64
    constexpr Base base = { .value = 0 };
    constexpr Limit limit = { .value = 0 };

    createEntry(gdt.KernelCode(), base, limit, true, 0);
    createEntry(gdt.KernelData(), base, limit, false, 0);
    createEntry(gdt.UserCode(), base, limit, true, 3);
    createEntry(gdt.UserData(), base, limit, false, 3);

    commitAndFlush();
}

}
