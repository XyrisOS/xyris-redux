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

#include <Loader/Entry.hpp>
#include <Memory/MemoryLayout.hpp>
#include <limine.h>
#include <stddef.h>
#include <stdint.h>

// Variables

// Extern declarations for C++ global constructors.
// NOLINTBEGIN(*-reserved-identifier)
extern void (*__preinit_array[])();
extern void (*__preinit_array_end[])();
extern void (*__init_array[])();
extern void (*__init_array_end[])();
extern void (*__ctors[])();
extern void (*__ctors_end[])();
// NOLINTEND(*-reserved-identifier)

static volatile limine_framebuffer_request framebufferRequest = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr,
};

namespace Loader
{

static void callConstructors(void (**begin)(), void (**end)())
{
    for (auto constructor = begin; constructor != end; constructor++) {
        (*constructor)();
    }
}

static void callLegacyConstructors(void (**begin)(), void (**end)())
{
    // Legacy .ctors are conventionally run in reverse order.
    for (auto constructor = end; constructor != begin;) {
        (*--constructor)();
    }
}

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
    Loader::callConstructors(__preinit_array, __preinit_array_end);
    Loader::callConstructors(__init_array, __init_array_end);
    Loader::callLegacyConstructors(__ctors, __ctors_end);

    // Ensure we got a framebuffer.
    if (framebufferRequest.response == nullptr || framebufferRequest.response->framebuffer_count < 1) {
        Loader::HaltAndCatchFire();
    }

    Kernel::Entry();
}
}
