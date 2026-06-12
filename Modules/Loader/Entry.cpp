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

#include <include/Loader/Entry.hpp>
#include <limine.h>
#include <stddef.h>
#include <stdint.h>

// Variables

// Extern declarations for C++ global constructors.
extern void (*__init_array[])();
extern void (*__init_array_end[])();

static volatile limine_framebuffer_request framebufferRequest = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr,
};

namespace Loader
{

[[noreturn]]
void HaltAndCatchFire()
{
    asm volatile("cli");
    while (true) {
        asm volatile("hlt");
    }
}

void ShowProgress(const uint32_t color)
{
    // Fetch the first framebuffer.
    const limine_framebuffer* framebuffer = framebufferRequest.response->framebuffers[0];

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    auto* fb_ptr = static_cast<uint32_t*>(framebuffer->address);
    for (size_t x = 0; x < 100; x++) {
        for (size_t y = 0; y < 100; y++) {
            fb_ptr[y * (framebuffer->pitch / 4) + x] = color;
        }
    }
}

}

extern "C" {

void LoaderEntry(void);

void LoaderEntry(void)
{
    // Call all C++ global constructors before doing anything else.
    for (size_t i = 0; &__init_array[i] != __init_array_end; i++) {
        __init_array[i]();
    }
    // Ensure we got a framebuffer.
    if (framebufferRequest.response == nullptr || framebufferRequest.response->framebuffer_count < 1) {
        Loader::HaltAndCatchFire();
    }

    Kernel::Entry();
}
}
