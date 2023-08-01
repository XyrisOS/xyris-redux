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

static void hcf(void);

static void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

void kernel_main(void) {
    hcf();
}
