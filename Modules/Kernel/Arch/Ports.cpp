/**
 * @file Ports.cpp
 * @author Keeton Feavel (kfeavel@xyr.is)
 * @brief CPU port access functions
 * @version 0.3
 * @date 2023-08-23
 *
 * @copyright Copyright the Xyris Contributors (c) 2020
 *
 */
#include "Ports.hpp"

namespace Ports
{

    uint8_t readByte(uint16_t port)
    {
        uint8_t result;
        asm volatile("inb %1, %0"
                     : "=a"(result)
                     : "Nd"(port));
        return result;
    }

    void writeByte(uint16_t port, uint8_t data)
    {
        asm volatile("outb %0, %1"
                     :
                     : "a"(data), "Nd"(port));
    }

    uint16_t readWord(uint16_t port)
    {
        uint16_t result;
        asm volatile("inw %1, %0"
                     : "=a"(result)
                     : "Nd"(port));
        return result;
    }

    void writeWord(uint16_t port, uint16_t data)
    {
        asm volatile("outw %0, %1"
                     :
                     : "a"(data), "Nd"(port));
    }

    uint32_t readLong(uint16_t port)
    {
        uint32_t result;
        asm volatile("inl %1, %0"
                     : "=a"(result)
                     : "Nd"(port));
        return result;
    }

    void writeLong(uint16_t port, uint32_t data)
    {
        asm volatile("outl %0, %1"
                     :
                     : "a"(data), "Nd"(port));
    }

}
