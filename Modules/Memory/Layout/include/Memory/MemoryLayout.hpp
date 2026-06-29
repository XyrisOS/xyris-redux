/**
 * @file MemoryLayout.hpp
 * @author Keeton Feavel (keeton@xyr.is)
 * @brief Kernel loader translation layer
 * @version 0.1
 * @date 2026-06-28
 *
 * @copyright Copyright (c) 2026
 *
 */

#pragma once
#include <stddef.h>
#include <stdint.h>

namespace Memory::Layout
{

struct SectionType {
    enum class Value : uint8_t {
        Available,
        Reserved,
        AcpiReclaimable,
        NonVolatileSleepingMemory,
        BadMemory,
        BootloaderReclaimable,
        KernelAndModules,
        Framebuffer,
        Unknown,
    };

    Value value;

    explicit SectionType(const Value v)
        : value(v)
    {
    }

    explicit operator Value() const
    {
        return value;
    }

    [[nodiscard]]
    const char* Name() const
    {
        switch (value) {
            case Value::Available:
                return "Available";
            case Value::Reserved:
                return "Reserved";
            case Value::AcpiReclaimable:
                return "ACPI Reclaimable";
            case Value::NonVolatileSleepingMemory:
                return "Non-Volatile Sleeping Memory";
            case Value::BadMemory:
                return "Bad";
            case Value::BootloaderReclaimable:
                return "Bootloader Reclaimable";
            case Value::KernelAndModules:
                return "Kernel";
            case Value::Framebuffer:
                return "Framebuffer";
            case Value::Unknown:
                return "Unknown";
            // This should never, ever happen.
            default:
                return nullptr;
        }
    }
};

class Section {
    const uint64_t base;
    const uint64_t length;
    const SectionType type;

public:
    Section(const uint64_t base, const uint64_t length, const SectionType type)
        : base(base)
        , length(length)
        , type(type)
    {
    }

    [[nodiscard]]
    constexpr uint64_t Base() const
    {
        return base;
    }

    [[nodiscard]]
    constexpr uint64_t End() const
    {
        return base + length;
    }

    [[nodiscard]]
    constexpr bool Empty() const
    {
        return length == 0;
    }

    [[nodiscard]]
    constexpr bool Contains(const uint64_t address) const
    {
        return base <= address && address < End();
    }

    [[nodiscard]]
    constexpr SectionType Type() const
    {
        return type;
    }
};

class Layout {
    const Section* sections;
    const size_t count;

public:
    constexpr Layout(const Section* sections, const size_t count)
        : sections(sections)
        , count(count)
    {
    }

    [[nodiscard]]
    constexpr const Section* Sections() const
    {
        return sections;
    }

    [[nodiscard]]
    constexpr size_t Count() const
    {
        return count;
    }
};

}