#include <StackProtector.hpp>

#include "HaltAndCatchFire.hpp"

extern "C" {

uintptr_t __stack_chk_guard = 0x595249535354414B;

[[noreturn]]
void __stack_chk_fail(void)
{
    Runtime::HaltAndCatchFire();
}

}