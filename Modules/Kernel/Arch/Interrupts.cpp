/**
 * @file Interrupts.cpp
 * @author Keeton Feavel (kfeavel@xyr.is)
 * @brief
 * @version 0.1
 * @date 2023-08-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Interrupts.hpp"

namespace Interrupts
{

void Enable()
{
    asm volatile("sti");
}

void Disable()
{
    asm volatile("cli");
}

}
