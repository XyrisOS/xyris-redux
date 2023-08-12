/**
 * @file Entry.cpp
 * @author Keeton Feavel (keeton@xyr.is)
 * @brief Kernel entry point
 * @version 0.1
 * @date 2023-07-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Entry.hpp>
#include <Arch.hpp>

namespace Kernel
{

void Entry(void) {
    Arch::Initialize();
    Loader::ShowProgress();

    // Loop on tasks

    Loader::HaltAndCatchFire();
}

}
