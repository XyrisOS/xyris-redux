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

#include <Loader/Entry.hpp>
#include <Arch/Arch.hpp>

namespace Kernel
{

[[noreturn]]
void Entry() {
    Arch::Initialize();

    // Loop on tasks
    constexpr uint32_t colors[] = { 0xFF00FF, 0xFFFF00, 0x00FFFF };
    while (true) {
        for (const unsigned int color : colors) {
            Loader::ShowProgress(color);
        }
    }

    Arch::HaltAndCatchFire();
}

}
