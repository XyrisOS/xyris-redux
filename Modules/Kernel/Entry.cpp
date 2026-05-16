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
#include <HaltAndCatchFire.hpp>

extern "C" void KernelSwiftRun(void);

namespace Kernel
{

void Entry(void) {
    Arch::Initialize();
    KernelSwiftRun();

    // Loop on tasks

    Runtime::HaltAndCatchFire();
}

}
