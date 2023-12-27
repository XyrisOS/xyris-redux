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

// Types

union Limit {
    struct [[gnu::packed]] LimitSections
    {
        int low     : 16;
        int high    : 4;
    } section;
    uint32_t value;
};

union Base {
    struct [[gnu::packed]] BaseSections
    {
        int low     : 24;
        int high    : 8;
    } section;
    uint32_t value;
};

struct [[gnu::packed]] Entry {
    // Limit
    int limitLow        : 16;
    // Base
    int baseLow         : 24;
    // Access byte
    int accessed        : 1;    // Accessed indicator (default to 0)
    int rw              : 1;    // Readable (code segment) / writeable bit (data segment)
    int dc              : 1;    // Conforming (code segment) / direction (data segment)
    int executable      : 1;    // Code (1) or data (0)
    int system          : 1;    // Task segment (0) or code/data segment (1)
    int privilege       : 2;    // Privilege level (rings 0-3)
    int present         : 1;    // Indicates entry is available (default to 1)
    // Limit
    int limitHigh       : 4;    // Ignored
    // Flags
    int reserved        : 1;    // Reserved (default to 0)
    int longMode        : 1;    // Indicates a long mode (64-bit) code segment if set
    int size            : 1;    // Indicates a 32-bit (1) or 16-bit (0) protected mode segment
    int granulatity     : 1;    // Indicates page granularity if set (otherwise byte granularity)
    // Base
    int baseHigh        : 8;
};

struct [[gnu::packed]] GDT {
    Entry& kernelNull() { return entries[0]; }
    Entry& kernelCode() { return entries[1]; }
    Entry& kernelData() { return entries[2]; }
    Entry& userNull() { return entries[3]; }
    Entry& userCode() { return entries[4]; }
    Entry& userData() { return entries[5]; }

    void* address() { return entries; }

    Entry entries[6] = {
        Entry(),    // Kernel null
        Entry(),    // Kernel code
        Entry(),    // Kernel data
        Entry(),    // User null
        Entry(),    // User code
        Entry()     // User data
    };
};

// Cannot `static_assert` `Limit` because of irrational byte size
static_assert(sizeof(Base) == 4, "Base size assertion failure");
static_assert(sizeof(Entry) == 8, "Entry size assertion failure");
static_assert(sizeof(GDT) == sizeof(Entry) * 6, "GDT size assertion failure");

struct [[gnu::packed]] GDTR {
    uint16_t size;
    void* addr;
};

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
        .executable = (executable ? 1 : 0),
        .system = 1,
        .privilege = privilege,
        .present = 1,
        .limitHigh = limit.section.high,
        .reserved = 0,
        .longMode = 0,
        .size = 1,
        .granulatity = 1,
        .baseHigh = base.section.high,
    };
}

void Initialize(void)
{
    // Base and limit are the same for all entries on x86_64
    const union Base base = { .value = 0 };
    const union Limit limit = { .value = 0x000FFFFF };

    CreateEntry(gdt.kernelCode(), base, limit, true, 0);
    CreateEntry(gdt.kernelData(), base, limit, false, 0);
    CreateEntry(gdt.userCode(), base, limit, true, 3);
    CreateEntry(gdt.userData(), base, limit, false, 3);

    CommitAndFlush();
}

}
