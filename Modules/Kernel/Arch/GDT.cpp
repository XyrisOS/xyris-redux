/**
 * @file GDT.cpp
 * @author Keeton Feavel (keeton@xyr.is)
 * @brief
 * @version 0.1
 * @date 2023-08-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "GDT.hpp"
#include <stddef.h>

// Implemented by GDT.asm
extern "C" void GDT_Flush(void* pGDTR);

namespace GDT
{

// Types

struct GDTR {
    uint16_t size   : 16;
    uintptr_t addr  : 64;
} __attribute__((packed));

union Limit {
    struct [[gnu::packed]] LimitSections
    {
        uint16_t low : 16;
        uint8_t high : 4;
    } section;
    uint32_t value;
};

union Base {
    struct [[gnu::packed]] BaseSections
    {
        uint32_t low : 24;
        uint8_t high : 8;
    } section;
    uint32_t value;
};

struct [[gnu::packed]] Entry {
    // Limit
    uint16_t limitLow       : 16;
    // Base
    uint32_t baseLow        : 24;
    // Access byte
    uint8_t accessed        : 1;    // Accessed indicator (default to 0)
    uint8_t rw              : 1;    // Readable (code segment) / writeable bit (data segment)
    uint8_t dc              : 1;    // Conforming (code segment) / direction (data segment)
    uint8_t executable      : 1;    // Code (1) or data (0)
    uint8_t system          : 1;    // Task segment (0) or code/data segment (1)
    uint8_t privilege       : 2;    // Privilege level (rings 0-3)
    uint8_t present         : 1;    // Indicates entry is available (default to 1)
    // Limit
    uint8_t limitHigh       : 4;    // Ignored
    // Flags
    uint8_t reserved        : 1;    // Reserved (default to 0)
    uint8_t longMode        : 1;    // Indicates a long mode (64-bit) code segment if set
    uint8_t size            : 1;    // Indicates a 32-bit (1) or 16-bit (0) protected mode segment
    uint8_t granulatity     : 1;    // Indicates page granularity if set (otherwise byte granularity)
    // Base
    uint8_t baseHigh        : 8;
};

// Variables

const size_t gdtMaxEntries = 5;
struct Entry gdt[gdtMaxEntries];
struct GDTR gdtr;

// Functions

static void CommitAndFlush(void)
{
    // Update GDT register and flush
    gdtr.size = sizeof(gdt) - 1;
    gdtr.addr = reinterpret_cast<uintptr_t>(&gdt);
    GDT_Flush(reinterpret_cast<void*>(&gdtr));
}


void Initialize(void)
{
    size_t index = 0;
    const union Base base = { .value = 0 };
    const union Limit limit = { .value = 0x000FFFFF };

    // Null segment

    gdt[index++] = Entry();

    // Kernel

    gdt[index++] = {
        .limitLow = limit.section.low,
        .baseLow = base.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 1,
        .system = 1,
        .privilege = 0,
        .present = 1,
        .limitHigh = limit.section.high,
        .reserved = 0,
        .longMode = 0,
        .size = 1,
        .granulatity = 1,
        .baseHigh = base.section.high,
    };

    gdt[index++] = {
        .limitLow = limit.section.low,
        .baseLow = base.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 0,
        .system = 1,
        .privilege = 0,
        .present = 1,
        .limitHigh = limit.section.high,
        .reserved = 0,
        .longMode = 0,
        .size = 1,
        .granulatity = 1,
        .baseHigh = base.section.high,
    };

    // Userspace

    gdt[index++] = {
        .limitLow = limit.section.low,
        .baseLow = base.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 1,
        .system = 1,
        .privilege = 3,
        .present = 1,
        .limitHigh = limit.section.high,
        .reserved = 0,
        .longMode = 0,
        .size = 1,
        .granulatity = 1,
        .baseHigh = base.section.high,
    };

    gdt[index++] = {
        .limitLow = limit.section.low,
        .baseLow = base.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 0,
        .system = 1,
        .privilege = 3,
        .present = 1,
        .limitHigh = limit.section.high,
        .reserved = 0,
        .longMode = 0,
        .size = 1,
        .granulatity = 1,
        .baseHigh = base.section.high,
    };

    CommitAndFlush();
}

}
