/**
 * @file IDT.hpp
 * @author Keeton Feavel (kfeavel@xyr.is)
 * @brief
 * @version 0.1
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once
#include <stdint.h>

namespace IDT
{

// Types
enum Gate : uint8_t {
    GateInterrupt   = 0xE,
    GateTrap        = 0xF,
};

struct __attribute__((packed)) OffsetSections {
    unsigned int low     : 16;
    unsigned int mid     : 16;
    unsigned int high    : 32;
};

union Offset {
    OffsetSections section;
    uintptr_t value;
};

struct __attribute__((packed)) SegmentSelectorSections {
    unsigned int privilege       : 2;   // Privilege level (rings 0-3)
    unsigned int type            : 1;   // Indicates the descriptor table is the GDT (1) or LDT (0)
    unsigned int index           : 13;  // Index into the table (type specified above)
};

union SegmentSelector {
    SegmentSelectorSections section;
    uint16_t value;
};

struct __attribute__((packed)) Entry {
    // Offset
    unsigned int offsetLow       : 16;
    // Segment Selector
    unsigned int selector        : 16;  // Segment selector
    // Interrupt Stack Table
    unsigned int stackTable      : 3;   // Interrupt stack table offset
    unsigned int reservedLow     : 4;   // Reserved bytes
    // Flags
    unsigned int type            : 4;   // Interrupt descriptor type
    unsigned int zero            : 1;   // Always zero
    unsigned int privilege       : 2;   // Privilege level (rings 0-3)
    unsigned int present         : 1;   // Indicates if the entry is valid (1) or invalid (0)
    // Offset
    unsigned int offsetMid       : 16;
    unsigned int offsetHigh      : 32;
    unsigned int reservedHigh    : 32;  // Reserved bytes

};

struct __attribute__((packed)) IDT {
    uintptr_t address() { return reinterpret_cast<uintptr_t>(&entries); }

    // Aligned for performance
    [[gnu::aligned(0x10)]] Entry entries[256];
};

struct __attribute__((packed)) IDTR {
    uint16_t size;
    uintptr_t addr;
};

static_assert(sizeof(Offset) == 8, "Offset size assertion failure");
static_assert(sizeof(SegmentSelector) == 2, "SegmentSelector size assertion failure");
static_assert(sizeof(IDT) == (sizeof(Entry) * 256), "IDT size assertion failure");
static_assert(sizeof(IDTR) == 10, "IDTR size assertion failure");

// Functions

void Initialize(void);

}
