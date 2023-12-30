/**
 * @file PIC.hpp
 * @author Keeton Feavel (kfeavel@xyr.is)
 * @brief 8259 Programmable Interrupt Controller
 * @version 0.1
 * @date 2023-12-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once
#include <stdint.h>

namespace PIC
{

// Functions

void Initialize(void);
void Finalize(void);

void EndOfInterrupt(uint64_t id);

}
