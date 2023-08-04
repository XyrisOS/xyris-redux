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

namespace GDT
{

const size_t gdtMaxEntries = 7;
struct Entry gdt[gdtMaxEntries];
struct GDTR gdtr;


static void Flush(void)
{
    // Update GDT register and flush
    gdtr.size = sizeof(gdt) - 1;
    gdtr.addr = reinterpret_cast<uintptr_t>(&gdt);
    asm volatile("lgdt %0" ::"m"(gdtr) : "memory");
}


void Initialize(void)
{
    // FIXME: Many of these segments are nearly identical.
    //        Create common definitions that can be used and
    //        tweaked a little to reduce duplicate code.

    // Null segment

    size_t index = 0;
    const union Base nullSegmentBase = { .value = 0 };
    const union Limit nullSegmentLimit = { .value = 0 };
    gdt[index++] = {
        .limitLow = nullSegmentLimit.section.low,
        .baseLow = nullSegmentBase.section.low,
        .accessed = 0,
        .rw = 0,
        .dc = 0,
        .executable = 0,
        .system = 0,
        .privilege = 0,
        .present = 0,
        .limitHigh = nullSegmentLimit.section.high,
        .reserved = 0,
        .longMode = 0,
        .size = 0,
        .granulatity = 0,
        .baseMid = nullSegmentBase.section.mid,
        .baseHigh = nullSegmentBase.section.high,
        .reservedHigh = 0,
    };

    // 16-bit segments

    const union Base realCodeBase = { .value = 0 };
    const union Limit realCodeLimit = { .value = 0x0000FFFF };
    gdt[index++] = {
        .limitLow = realCodeLimit.section.low,
        .baseLow = realCodeBase.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 1,
        .system = 1,
        .privilege = 0,
        .present = 1,
        .limitHigh = realCodeLimit.section.high,
        .reserved = 0,
        .longMode = 0,
        .size = 0,
        .granulatity = 0,
        .baseMid = realCodeBase.section.mid,
        .baseHigh = realCodeBase.section.high,
        .reservedHigh = 0,
    };

    const union Base realDataBase = { .value = 0 };
    const union Limit realDataLimit = { .value = 0x0000FFFF };
    gdt[index++] = {
        .limitLow = realDataLimit.section.low,
        .baseLow = realDataBase.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 0,
        .system = 1,
        .privilege = 0,
        .present = 1,
        .limitHigh = realDataLimit.section.high,
        .reserved = 0,
        .longMode = 0,
        .size = 0,
        .granulatity = 0,
        .baseMid = realDataBase.section.mid,
        .baseHigh = realDataBase.section.high,
        .reservedHigh = 0,
    };

    // 32-bit segments

    const union Base protectedCodeBase = { .value = 0 };
    const union Limit protectedCodeLimit = { .value = 0xFFFFFFFF };
    gdt[index++] = {
        .limitLow = protectedCodeLimit.section.low,
        .baseLow = protectedCodeBase.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 1,
        .system = 1,
        .privilege = 0,
        .present = 1,
        .limitHigh = protectedCodeLimit.section.high,
        .reserved = 0,
        .longMode = 0,
        .size = 1,
        .granulatity = 1,
        .baseMid = protectedCodeBase.section.mid,
        .baseHigh = protectedCodeBase.section.high,
        .reservedHigh = 0,
    };

    const union Base protectedDataBase = { .value = 0 };
    const union Limit protectedDataLimit = { .value = 0xFFFFFFFF };
    gdt[index++] = {
        .limitLow = protectedDataLimit.section.low,
        .baseLow = protectedDataBase.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 0,
        .system = 1,
        .privilege = 0,
        .present = 1,
        .limitHigh = protectedDataLimit.section.high,
        .reserved = 0,
        .longMode = 0,
        .size = 1,
        .granulatity = 1,
        .baseMid = protectedDataBase.section.mid,
        .baseHigh = protectedDataBase.section.high,
        .reservedHigh = 0,
    };

    // 64-bit segments

    const union Base longCodeBase = { .value = 0 };
    const union Limit longCodeLimit = { .value = 0 };
    gdt[index++] = {
        .limitLow = longCodeLimit.section.low,
        .baseLow = longCodeBase.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 1,
        .system = 1,
        .privilege = 0,
        .present = 1,
        .limitHigh = longCodeLimit.section.high,
        .reserved = 0,
        .longMode = 1,
        .size = 0,
        .granulatity = 0,
        .baseMid = longCodeBase.section.mid,
        .baseHigh = longCodeBase.section.high,
        .reservedHigh = 0,
    };

    const union Base longDataBase = { .value = 0 };
    const union Limit longDataLimit = { .value = 0 };
    gdt[index++] = {
        .limitLow = longDataLimit.section.low,
        .baseLow = longDataBase.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 1,
        .system = 1,
        .privilege = 0,
        .present = 1,
        .limitHigh = longDataLimit.section.high,
        .reserved = 0,
        .longMode = 0,
        .size = 0,
        .granulatity = 0,
        .baseMid = longDataBase.section.mid,
        .baseHigh = longDataBase.section.high,
        .reservedHigh = 0,
    };

    Flush();
}

}
