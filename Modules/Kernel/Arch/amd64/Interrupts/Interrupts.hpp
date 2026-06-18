/**
 * @file Interrupts.hpp
 * @author Keeton Feavel (kfeavel@xyr.is)
 * @brief
 * @version 0.1
 * @date 2023-08-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once
#include <CPU/FlagsR.hpp>

namespace Interrupts
{

void Enable();
void Disable();

template<typename T>
void CriticalRegion(T work)
{
    // Safely handle interrupts already being disabled during critical section
    // by saving FLAGS register contents and checking if interrupts were enabled
    // before re-enabling upon critical work exit.
    Registers::FlagsR flags = { .value = 0 };
    asm volatile(
        "pushfq\n"
        "pop %0\n"
        "cli"
        : "=r"(flags.value)
        :
        : "memory");

    work();

    if (flags.section.interruptEnable != 0) {
        Enable();
    }
}

}
