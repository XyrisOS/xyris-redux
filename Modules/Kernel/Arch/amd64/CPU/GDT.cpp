/**
 * @file GDT.cpp
 * @author Keeton Feavel (kfeavel@xyr.is)
 * @brief
 * @version 0.1
 * @date 2023-08-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "GDT.hpp"

namespace Arch::GDT
{

// Variables

static auto gdt = GDT();
static auto gdtr = GDTR();

// Functions

// Implemented by GDT.asm
extern "C" void FlushGDT(GDTR* pGDTR);

static void commitAndFlush()
{
    // Update GDT register and flush
    gdtr = {
        .size = sizeof(gdt) - 1,
        .addr = gdt.Address(),
    };

    FlushGDT(&gdtr);
}

void Initialize()
{
    commitAndFlush();
}

}
