/**
 * @file Entry.cpp
 * @author Keeton Feavel (keeton@xyr.is)
 * @brief Kernel loader translation layer
 * @version 0.1
 * @date 2023-07-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Entry.hpp>
#include <stddef.h>
#include <stdint.h>
#include <limine.h>

static volatile struct limine_framebuffer_request framebufferRequest = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = NULL,
};


extern "C" void LoaderEntry(void);


namespace Loader {

[[noreturn]]
static void HaltAndCatchFire(void) {
    asm volatile ("cli");
    while (true) {
        asm volatile ("hlt");
    }
}

void ShowProgress(uint32_t color)
{
    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebufferRequest.response->framebuffers[0];

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    uint32_t* fb_ptr = reinterpret_cast<uint32_t*>(framebuffer->address);
    for (size_t x = 0; x < 100; x++) {
        for (size_t y = 0; y < 100; y++) {
            fb_ptr[y * (framebuffer->pitch / 4) + x] = color;
        }
    }
}

}

extern "C" void LoaderEntry(void) {
    // Ensure we got a framebuffer.
    if (framebufferRequest.response == NULL || framebufferRequest.response->framebuffer_count < 1) {
        Loader::HaltAndCatchFire();
    }

    Kernel::Entry();
}
