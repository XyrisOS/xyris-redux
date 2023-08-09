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

const size_t gdtMaxEntries = 5;
struct Entry gdt[gdtMaxEntries];
struct GDTR gdtr;


static void CommitAndFlush(void)
{
    // Update GDT register and flush
    gdtr.size = sizeof(gdt) - 1;
    gdtr.addr = reinterpret_cast<uintptr_t>(&gdt);
    GDT_Flush(reinterpret_cast<void*>(&gdtr));
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
        .baseHigh = nullSegmentBase.section.high,
    };

    const union Base kernelCodeBase = { .value = 0 };
    const union Limit kernelCodeLimit = { .value = 0x000FFFFF };
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
        .longMode = 0,
        .size = 1,
        .granulatity = 1,
        .baseHigh = kernelCodeBase.section.high,
    };

    const union Base kernelDataBase = { .value = 0 };
    const union Limit kernelDataLimit = { .value = 0x000FFFFF };
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
        .baseHigh = kernelDataBase.section.high,
    };

    const union Base userCodeBase = { .value = 0 };
    const union Limit userCodeLimit = { .value = 0x000FFFFF };
    gdt[index++] = {
        .limitLow = userCodeLimit.section.low,
        .baseLow = userCodeBase.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 1,
        .system = 1,
        .privilege = 3,
        .present = 1,
        .limitHigh = userCodeLimit.section.high,
        .reserved = 0,
        .longMode = 0,
        .size = 1,
        .granulatity = 1,
        .baseHigh = userCodeBase.section.high,
    };

    const union Base userDataBase = { .value = 0 };
    const union Limit userDataLimit = { .value = 0x000FFFFF };
    gdt[index++] = {
        .limitLow = userDataLimit.section.low,
        .baseLow = userDataBase.section.low,
        .accessed = 0,
        .rw = 1,
        .dc = 0,
        .executable = 0,
        .system = 1,
        .privilege = 3,
        .present = 1,
        .limitHigh = userDataLimit.section.high,
        .reserved = 0,
        .longMode = 0,
        .size = 1,
        .granulatity = 1,
        .baseHigh = userDataBase.section.high,
    };

    CommitAndFlush();
}

}
