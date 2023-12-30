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

void Enable(void);
void Disable(void);

template<typename T>
void CriticalRegion(T work)
{
    Disable();
    work();
    Enable();
}

}
