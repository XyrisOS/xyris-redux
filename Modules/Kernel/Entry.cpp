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
#include <stdint.h>

namespace Kernel
{

void Entry(void) {
    Arch::Initialize();

    // Loop on tasks
    while (true) {
        for (uint32_t c = 0x000000; c <= 0xffffff; c++) {
            Loader::ShowProgress(c);
        }
    }

    Arch::HaltAndCatchFire();
}

}
