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
extern "C" void FlushGDT();

namespace GDT
{

const size_t gdtMaxEntries = 5;
struct Entry gdt[gdtMaxEntries];
struct GDTR gdtr;


static void Flush(void)
{
    // Update GDT register and flush
    gdtr.size = sizeof(gdt) - 1;
    gdtr.addr = reinterpret_cast<uintptr_t>(&gdt);
    asm volatile("lgdt %0" ::"m"(gdtr) : "memory");
    FlushGDT();
}


void Initialize(void)
{
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

    // Kernel

    const union Base kernelCodeBase = { .value = 0 };
    const union Limit kernelCodeLimit = { .value = 0x00FFFFFF };
    gdt[index++] = {
        .limitLow = kernelCodeLimit.section.low,
        .baseLow = kernelCodeBase.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 1,
        .system = 1,
        .privilege = 0,
        .present = 1,
        .limitHigh = kernelCodeLimit.section.high,
        .reserved = 0,
        .longMode = 1,
        .size = 0,
        .granulatity = 1,
        .baseMid = kernelCodeBase.section.mid,
        .baseHigh = kernelCodeBase.section.high,
        .reservedHigh = 0,
    };

    const union Base kernelDataBase = { .value = 0 };
    const union Limit kernelDataLimit = { .value = 0x00FFFFFF };
    gdt[index++] = {
        .limitLow = kernelDataLimit.section.low,
        .baseLow = kernelDataBase.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 0,
        .system = 1,
        .privilege = 0,
        .present = 1,
        .limitHigh = kernelDataLimit.section.high,
        .reserved = 0,
        .longMode = 0,
        .size = 1,
        .granulatity = 1,
        .baseMid = kernelDataBase.section.mid,
        .baseHigh = kernelDataBase.section.high,
        .reservedHigh = 0,
    };

    // Userspace

    const union Base userspaceCodeBase = { .value = 0 };
    const union Limit userspaceCodeLimit = { .value = 0x00FFFFFF };
    gdt[index++] = {
        .limitLow = userspaceCodeLimit.section.low,
        .baseLow = userspaceCodeBase.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 1,
        .system = 1,
        .privilege = 3,
        .present = 1,
        .limitHigh = userspaceCodeLimit.section.high,
        .reserved = 0,
        .longMode = 1,
        .size = 0,
        .granulatity = 1,
        .baseMid = userspaceCodeBase.section.mid,
        .baseHigh = userspaceCodeBase.section.high,
        .reservedHigh = 0,
    };

    const union Base userspaceDataBase = { .value = 0 };
    const union Limit userspaceDataLimit = { .value = 0xFFFF };
    gdt[index++] = {
        .limitLow = userspaceDataLimit.section.low,
        .baseLow = userspaceDataBase.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 0,
        .system = 1,
        .privilege = 3,
        .present = 1,
        .limitHigh = userspaceDataLimit.section.high,
        .reserved = 0,
        .longMode = 0,
        .size = 1,
        .granulatity = 1,
        .baseMid = userspaceDataBase.section.mid,
        .baseHigh = userspaceDataBase.section.high,
        .reservedHigh = 0,
    };

    Flush();
}

}
