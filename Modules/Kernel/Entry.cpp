/**
 * @file Entry.cpp
 * @author Keeton Feavel (kfeavel@xyr.is)
 * @brief Kernel entry point
 * @version 0.1
 * @date 2023-07-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Entry.hpp>
#include <Arch.hpp>
#include <stddef.h>

namespace Kernel
{

void Entry(void) {
    Arch::Initialize();

    // Loop on tasks
    const uint32_t colors[] = { 0xFF00FF, 0xFFFF00, 0x00FFFF };
    while (true) {
        for (size_t i = 0; i < (sizeof(colors) / sizeof(colors[0])); i++) {
            Loader::ShowProgress(colors[i]);
        }
    }

    Arch::HaltAndCatchFire();
}

}
