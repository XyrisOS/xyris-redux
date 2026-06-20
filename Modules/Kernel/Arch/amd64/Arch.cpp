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

#include <Arch/Arch.hpp>
#include "CPU/GDT.hpp"
#include "CPU/TSS.hpp"
#include "Devices/PIC.hpp"
#include "Interrupts/IDT.hpp"
#include "Interrupts/Interrupts.hpp"

namespace Arch
{

void Initialize()
{
    Interrupts::CriticalRegion([] {
        PIC::Initialize();
        TSS::Initialize();
        GDT::Initialize();
        IDT::Initialize();
    });
}

[[noreturn]]
void HaltAndCatchFire()
{
    Interrupts::Disable();
    while (true) {
        asm volatile("hlt");
    }
}

}
