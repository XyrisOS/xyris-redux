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
void HaltAndCatchFire(void) {
    asm volatile ("cli");
    while (true) {
        asm volatile ("hlt");
    }
}


void ShowProgress(void)
{
    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebufferRequest.response->framebuffers[0];

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    for (size_t i = 100; i > 0; i--) {
        uint32_t* fb_ptr = reinterpret_cast<uint32_t*>(framebuffer->address);
        fb_ptr[i] = 0xff0000;
    }
}

}


extern "C" void LoaderEntry(void) {
    // Ensure we got a framebuffer.
    if (framebufferRequest.response == NULL || framebufferRequest.response->framebuffer_count < 1) {
        Loader::HaltAndCatchFire();
    }

    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebufferRequest.response->framebuffers[0];

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    for (size_t i = 0; i < 100; i++) {
        uint32_t* fb_ptr = reinterpret_cast<uint32_t*>(framebuffer->address);
        fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff;
    }

    Kernel::Entry();
}
