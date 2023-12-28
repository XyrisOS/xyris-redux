/**
 * @file GDT.hpp
 * @author Keeton Feavel (keeton@xyr.is)
 * @brief
 * @version 0.1
 * @date 2023-08-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once
#include <stdint.h>

namespace GDT
{

// Types

struct __attribute__((packed)) LimitSections {
    unsigned int low     : 16;
    unsigned int high    : 4;
};

struct __attribute__((packed)) BaseSections {
    unsigned int low     : 24;
    unsigned int high    : 8;
};

union Limit {
    LimitSections section;
    uint32_t value;
};

union Base {
    BaseSections section;
    uint32_t value;
};

struct __attribute__((packed)) Entry {
    // Limit
    unsigned int limitLow        : 16;
    // Base
    unsigned int baseLow         : 24;
    // Access byte
    unsigned int accessed        : 1;    // Accessed indicator (default to 0)
    unsigned int rw              : 1;    // Readable (code segment) / writeable bit (data segment)
    unsigned int dc              : 1;    // Conforming (code segment) / direction (data segment)
    unsigned int executable      : 1;    // Code (1) or data (0)
    unsigned int system          : 1;    // Task segment (0) or code/data segment (1)
    unsigned int privilege       : 2;    // Privilege level (rings 0-3)
    unsigned int present         : 1;    // Indicates entry is available (default to 1)
    // Limit
    unsigned int limitHigh       : 4;    // Ignored
    // Flags
    unsigned int reserved        : 1;    // Reserved (default to 0)
    unsigned int longMode        : 1;    // Indicates a long mode (64-bit) code segment if set
    unsigned int size            : 1;    // Indicates a 32-bit (1) or 16-bit (0) protected mode segment
    unsigned int granulatity     : 1;    // Indicates page granularity if set (otherwise byte granularity)
    // Base
    unsigned int baseHigh        : 8;
};

struct __attribute__((packed)) GDT {
    Entry& kernelNull() { return entries[0]; }
    Entry& kernelCode() { return entries[1]; }
    Entry& kernelData() { return entries[2]; }
    Entry& userNull() { return entries[3]; }
    Entry& userCode() { return entries[4]; }
    Entry& userData() { return entries[5]; }

    uintptr_t address() { return reinterpret_cast<uintptr_t>(&entries); }

    Entry entries[6] = {
        Entry(),    // Kernel null
        Entry(),    // Kernel code
        Entry(),    // Kernel data
        Entry(),    // User null
        Entry(),    // User code
        Entry()     // User data
    };
};

struct __attribute__((packed)) GDTR {
    uint16_t size;
    uintptr_t addr;
};

// Cannot `static_assert` `Limit` because of irrational byte size
static_assert(sizeof(Base) == 4, "Base size assertion failure");
static_assert(sizeof(Entry) == 8, "Entry size assertion failure");
static_assert(sizeof(GDT) == (sizeof(Entry) * 6), "GDT size assertion failure");
static_assert(sizeof(GDTR) == 10, "GDTR size assertion failure");

// Functions

void Initialize(void);

}
