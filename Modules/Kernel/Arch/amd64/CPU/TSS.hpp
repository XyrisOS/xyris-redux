/**
 * @file TSS.hpp
 * @author Keeton Feavel (kfeavel@xyr.is)
 * @brief
 * @version 0.1
 * @date 2026-06-17
 *
 * @copyright Copyright (c) 2026
 *
 */

#pragma once
#include <stdint.h>

namespace Arch::TSS
{

// Types

/** TSS for storing interrupt stack pointers */
struct __attribute__((packed)) TSS {
    uint32_t reserved0;
    uint64_t rsp0;          // Stack pointer for privilege level 0
    uint64_t rsp1;          // Stack pointer for privilege level 1
    uint64_t rsp2;          // Stack pointer for privilege level 2
    uint64_t reserved1;
    uint64_t ist1;          // Interrupt Stack Table pointer 1
    uint64_t ist2;          // Interrupt Stack Table pointer 2
    uint64_t ist3;          // Interrupt Stack Table pointer 3
    uint64_t ist4;          // Interrupt Stack Table pointer 4
    uint64_t ist5;          // Interrupt Stack Table pointer 5
    uint64_t ist6;          // Interrupt Stack Table pointer 6
    uint64_t ist7;          // Interrupt Stack Table pointer 7
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t ioPB;          // I/O Map Base Address

    TSS();
};

/** TSS entry to be loaded by the GDT to reference the TSS */
struct __attribute__((packed)) Entry {
    uint64_t limitLow    : 16;
    uint64_t baseLow     : 24;
    uint64_t type        : 4;
    uint64_t system      : 1;
    uint64_t privilege   : 2;
    uint64_t present     : 1;
    uint64_t limitHigh   : 4;
    uint64_t available   : 1;
    uint64_t reservedLow : 2;
    uint64_t granularity : 1;
    uint64_t baseMid     : 8;

    uint32_t baseHigh;
    uint32_t reservedHigh;
};

static_assert(sizeof(TSS) == 104, "TSS size assertion failure");
static_assert(sizeof(Entry) == 16, "TSS entry size assertion failure");

// Functions

/** Creates and returns an Entry to be loaded into the GDT */
Entry CreateEntry();

}
