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
#include "TSS.hpp"
#include <stddef.h>
#include <stdint.h>

namespace Arch::GDT
{

// Types

struct __attribute__((packed)) LimitSections {
    unsigned int low  : 16;
    unsigned int high : 4;
};

struct __attribute__((packed)) BaseSections {
    unsigned int low  : 24;
    unsigned int high : 8;
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
    // Base and limit are the same for all entries on x86_64
    static constexpr Base base = { .value = 0 };
    static constexpr Limit limit = { .value = 0 };

    // Limit
    unsigned int limitLow    : 16;
    // Base
    unsigned int baseLow     : 24;
    // Access byte
    unsigned int accessed    : 1;    // Accessed indicator (default to 0)
    unsigned int rw          : 1;    // Readable (code segment) / writeable bit (data segment)
    unsigned int dc          : 1;    // Conforming (code segment) / direction (data segment)
    unsigned int executable  : 1;    // Code (1) or data (0)
    unsigned int system      : 1;    // Task segment (0) or code/data segment (1)
    unsigned int privilege   : 2;    // Privilege level (rings 0-3)
    unsigned int present     : 1;    // Indicates entry is available (default to 1)
    // Limit
    unsigned int limitHigh   : 4;    // Ignored
    // Flags
    unsigned int reserved    : 1;    // Reserved (default to 0)
    unsigned int longMode    : 1;    // Indicates a long mode (64-bit) code segment if set
    unsigned int size        : 1;    // Indicates a 32-bit (1) or 16-bit (0) protected mode segment
    unsigned int granularity : 1;    // Indicates page granularity if set (otherwise byte granularity)
    // Base
    unsigned int baseHigh    : 8;

    static constexpr Entry Null()
    {
        return {};
    }

    static constexpr Entry Code(const uint8_t descriptorPrivilege)
    {
        return { true, descriptorPrivilege };
    }

    static constexpr Entry Data(const uint8_t descriptorPrivilege)
    {
        return { false, descriptorPrivilege };
    }

private:
    constexpr Entry() = default;

    Entry(const bool isExecutable, const uint8_t privilegeLevel)
        : limitLow(limit.section.low)
        , baseLow(base.section.low)
        , accessed(0)
        , rw(1)
        , dc(0)
        , executable(static_cast<unsigned int>(isExecutable ? 1 : 0))
        , system(1)
        , privilege(privilegeLevel)
        , present(1)
        , limitHigh(limit.section.high)
        , reserved(0)
        , longMode(static_cast<unsigned int>(isExecutable ? 1 : 0))
        , size(0)
        , granularity(1)
        , baseHigh(base.section.high)
    {
    }
};

struct __attribute((packed)) Entries {
    // Made available for other services like the IDT
    static constexpr size_t KernelNullIndex() { return 0; }
    static constexpr size_t KernelCodeIndex() { return 1; }
    static constexpr size_t KernelDataIndex() { return 2; }
    static constexpr size_t UserNullIndex() { return 3; }
    static constexpr size_t UserCodeIndex() { return 4; }
    static constexpr size_t UserDataIndex() { return 5; }

    Entry entries[6] = {
        Entry::Null(),                 // Kernel null
        Entry::Code(0), // Kernel code
        Entry::Data(0), // Kernel data
        Entry::Null(),                 // User null
        Entry::Code(3), // User code
        Entry::Data(3)  // User data
    };

    // Additional TSS entry that's applied after the User Data section
    TSS::Entry tssEntry = TSS::CreateEntry();

    // Accessor functions to eliminate potential confusion
    Entry& KernelNull() { return entries[KernelNullIndex()]; }
    Entry& KernelCode() { return entries[KernelCodeIndex()]; }
    Entry& KernelData() { return entries[KernelDataIndex()]; }
    Entry& UserNull() { return entries[UserNullIndex()]; }
    Entry& UserCode() { return entries[UserCodeIndex()]; }
    Entry& UserData() { return entries[UserDataIndex()]; }
    TSS::Entry& TaskSelector() { return tssEntry; }
};

struct __attribute__((packed)) GDT {
    uintptr_t Address() { return reinterpret_cast<uintptr_t>(&entries); }
    // Combined GDT and TSS entries
    Entries entries = Entries();
};

struct __attribute__((packed)) GDTR {
    uint16_t size;
    uintptr_t addr;
};

// Cannot `static_assert` `Limit` because of irrational byte size
static_assert(sizeof(Base) == 4, "Base size assertion failure");
static_assert(sizeof(Entry) == 8, "Entry size assertion failure");
static_assert(sizeof(GDT) == (sizeof(Entry) * 8), "GDT size assertion failure");
static_assert(sizeof(GDTR) == 10, "GDTR size assertion failure");

// Functions

void Initialize();

}
