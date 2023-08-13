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

namespace Interrupts
{

void enable(void);
void disable(void);

template<typename Function>
void criticalRegion(Function criticalWork)
{
    disable();
    criticalWork();
    enable();
}

}
