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
#include "Interrupts.hpp"
#include "GDT.hpp"

namespace Arch
{

void Initialize(void)
{
    Interrupts::CriticalRegion([] {
        GDT::Initialize();
    });
}

}
