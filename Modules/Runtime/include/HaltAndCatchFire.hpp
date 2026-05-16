//
// Created by kfeavel on 5/16/26.
//

#pragma once

namespace Runtime
{

/**
 * @brief Early form of panic when no other option is available.
 *
 */
[[noreturn]]
void HaltAndCatchFire();

}
