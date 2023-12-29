/**
 * @file Arch.hpp
 * @author Keeton Feavel (keeton@xyr.is)
 * @brief
 * @version 0.1
 * @date 2023-08-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

namespace Arch
{

void Initialize(void);

/**
 * @brief Early form of panic when no other option is available.
 *
 */
[[noreturn]]
void HaltAndCatchFire(void);

}
