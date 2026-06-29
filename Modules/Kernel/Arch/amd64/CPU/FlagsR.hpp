#pragma once

#include <stdint.h>

namespace Arch::Registers
{

/** Bitwise structure definitions for contents of AMD64 FLAGS register */
struct __attribute__((packed)) Flags {
    uint64_t carry                   : 1;
    uint64_t reserved1               : 1;
    uint64_t parity                  : 1;
    uint64_t reserved3               : 1;
    uint64_t auxiliaryCarry          : 1;
    uint64_t reserved5               : 1;
    uint64_t zero                    : 1;
    uint64_t sign                    : 1;
    uint64_t trap                    : 1;
    uint64_t interruptEnable         : 1;
    uint64_t direction               : 1;
    uint64_t overflow                : 1;
    uint64_t ioPrivilegeLevel        : 2;
    uint64_t nestedTask              : 1;
    uint64_t reserved15              : 1;
    uint64_t resume                  : 1;
    uint64_t virtual8086             : 1;
    uint64_t alignmentCheck          : 1;
    uint64_t virtualInterrupt        : 1;
    uint64_t virtualInterruptPending : 1;
    uint64_t identification          : 1;
    uint64_t reservedHigh            : 42;
};

/** Union of Flags bit-wise structure and FLAGS literal `uint64_t` value */
union FlagsR {
    Flags section;
    uint64_t value;
};

static_assert(sizeof(FlagsR) == 8, "FlagsR size assertion failure");

}
