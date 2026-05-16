//
// Created by kfeavel on 5/16/26.
//

#include "HaltAndCatchFire.hpp"

namespace Runtime
{

[[noreturn]]
void HaltAndCatchFire() {
    asm volatile ("cli");
    while (true) {
        asm volatile ("hlt");
    }
}

}
