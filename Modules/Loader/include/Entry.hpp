/**
 * @file Entry.hpp
 * @author Keeton Feavel (keeton@xyr.is)
 * @brief Kernel loader translation layer
 * @version 0.1
 * @date 2023-07-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once
#include <stdint.h>

namespace Kernel
{

/**
 * @brief Kernel entry point symbol is required but not defined by Loader.
 * Whatever the Loader is linked against must provide a `Kernel::Entry` implementation.
 *
 */
void Entry();

}

namespace Loader
{

/**
 * @brief Early form of panic when no other option is available.
 *
 */
[[noreturn]]
void HaltAndCatchFire();

void ShowProgress(uint32_t color);

}
