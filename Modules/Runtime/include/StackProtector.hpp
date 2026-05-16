#pragma once

#include <stdint.h>

extern "C"
uintptr_t __stack_chk_guard;

extern "C"
[[noreturn]] void __stack_chk_fail(void);
