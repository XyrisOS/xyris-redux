/**
 * @file Arch.cpp
 * @author Keeton Feavel (keeton@xyr.is)
 * @brief
 * @version 0.1
 * @date 2023-08-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Arch.hpp"
#include "GDT.hpp"

namespace Arch
{

void Init(void)
{
    GDT::Init();
}

}
