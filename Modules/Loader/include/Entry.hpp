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
 * Whatever the Loader is linked against must provide a Kerne::Entry
 * implementation.
 *
 */
void Entry(void);

}

namespace Loader
{

void ShowProgress(uint32_t color);

}
