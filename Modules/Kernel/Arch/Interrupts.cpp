/**
 * @file Interrupts.cpp
 * @author Keeton Feavel (kfeavel@xyr.is)
 * @brief
 * @version 0.1
 * @date 2023-08-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Interrupts.hpp"
#include "Arch.hpp"
#include "PIC.hpp"
#include <stdint.h>

namespace Interrupts
{

// Types
struct InterruptFrame {
    // Pushed by `pushax` in ISR.asm
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    // Pushed by the CPU on interrupt
    uint64_t interrupt;
    uint64_t error;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

// Declaration in order to make compiler happy. We don't want to define
// this in the header though since we also don't want to define the
// interrupt frame structure there given that only ISR.asm should be
// calling into this.
extern "C" void InterruptHandler(InterruptFrame* frame);
extern "C" void InterruptHandler(InterruptFrame* frame)
{
    // Halt on exceptions for now. Will update this to panic later.
    if (frame->interrupt <= 32) {
        Arch::HaltAndCatchFire();
    }

    PIC::EndOfInterrupt(frame->interrupt);

    // TODO: Create way to register handlers and call into them here.
    //       Make sure to allow for priority vs. lazy handling.
}

// Functions

void Enable(void)
{
    asm volatile("sti");
}

void Disable(void)
{
    asm volatile("cli");
}

}
