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
#include <limine.h>
#include <stddef.h>
#include <stdint.h>

static volatile limine_framebuffer_request framebufferRequest = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr,
};

namespace Loader
{

[[noreturn]]
static void HaltAndCatchFire()
{
    asm volatile("cli");
    while (true) {
        asm volatile("hlt");
    }
}

void ShowProgress()
{
    // Fetch the first framebuffer.
    const limine_framebuffer* framebuffer = framebufferRequest.response->framebuffers[0];

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    for (size_t i = 0; i < 100; i++) {
        auto* fb_ptr = static_cast<uint32_t*>(framebuffer->address);
        fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff;
    }
}

}

extern "C" {

void LoaderEntry(void);

void LoaderEntry(void)
{
    // Ensure we got a framebuffer.
    if (framebufferRequest.response == nullptr || framebufferRequest.response->framebuffer_count < 1) {
        Loader::HaltAndCatchFire();
    }

    Kernel::Entry();
}
}
