/**
 * @file Arch.cpp
 * @author Keeton Feavel (kfeavel@xyr.is)
 * @brief
 * @version 0.1
 * @date 2023-08-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Arch.hpp"
#include "GDT.hpp"
#include "Interrupts.hpp"
#include "IDT.hpp"

namespace Arch
{

void Initialize()
{
    Interrupts::CriticalRegion([] {
        GDT::Initialize();
        IDT::Initialize();
    });
}

[[noreturn]]
void HaltAndCatchFire(void) {
    asm volatile ("cli");
    while (true) {
        asm volatile ("hlt");
    }
}

}
