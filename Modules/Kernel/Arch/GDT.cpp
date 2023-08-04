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

const size_t gdtMaxEntries = 6;
struct Entry gdt[gdtMaxEntries];

void Init(void)
{
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
        .baseHigher = nullSegmentBase.section.higher,
        .reservedHigh = 0,
    };
}

}
