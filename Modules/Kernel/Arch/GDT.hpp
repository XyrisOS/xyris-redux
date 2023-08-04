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
        uint32_t low    : 24;
        uint8_t high    : 8;
        uint32_t higher : 32;
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
    uint8_t limitHigh       : 4;
    // Flags
    uint8_t reserved        : 1;    // Reserved (default to 0)
    uint8_t longMode        : 1;    // Indicates a long mode (64-bit) code segment if set
    uint8_t size            : 1;    // Indicates a 32-bit (1) or 16-bit (0) protected mode segment
    uint8_t granulatity     : 1;    // Indicates page granularity if set (otherwise byte granularity)
    // Base
    uint8_t baseHigh        : 8;
    // x86_64 extended
    uint32_t baseHigher     : 32;
    uint32_t reservedHigh   : 32;   // x86_64 reserved space
};
static_assert(sizeof(struct Entry) == 16);

void Init(void);

}
