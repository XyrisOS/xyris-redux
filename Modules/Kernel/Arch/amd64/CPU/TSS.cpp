/**
* @file TSS.cpp
* @author Keeton Feavel (kfeavel@xyr.is)
* @brief
* @version 0.1
* @date 2026-06-17
*
* @copyright Copyright (c) 2026
*
*/

#include "TSS.hpp"

namespace Arch::TSS
{

// Types

enum Type : uint8_t {
    Available   = 0x9,
    Busy        = 0xB,
};

// Variables

static auto tss = TSS();

// Functions

TSS::TSS()
    : reserved0(0)
    , rsp0(0)
    , rsp1(0)
    , rsp2(0)
    , reserved1(0)
    , ist1(0)
    , ist2(0)
    , ist3(0)
    , ist4(0)
    , ist5(0)
    , ist6(0)
    , ist7(0)
    , reserved2(0)
    , reserved3(0)
    , ioPB(sizeof(TSS))
{
}

Entry CreateEntry()
{
    const auto base = reinterpret_cast<uintptr_t>(&tss);
    constexpr uint32_t limit = sizeof(TSS) - 1;

    return {
        .limitLow = limit & 0xFFFF,
        .baseLow = base & 0xFFFFFF,
        .type = Available,
        .system = 0,
        .privilege = 0,
        .present = 1,
        .limitHigh = (limit >> 16) & 0xF,
        .available = 0,
        .reservedLow = 0,
        .granularity = 0,
        .baseMid = (base >> 24) & 0xFF,
        .baseHigh = static_cast<uint32_t>(base >> 32),
        .reservedHigh = 0,
    };
}

}
