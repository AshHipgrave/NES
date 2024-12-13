#include "pch.h"
#include "System/Memory.h"
#include "Core/Core.h"
#include "CPU/OpCodes.h"
#include "CPU/6502.h"
#include "Core/Utils.h"

Memory::Memory()
{
    for (uint16_t i = 0; i < k_MaxMemory; i++)
    {
        m_MemoryBuffer[i] = 0;
    }
}

uint8_t Memory::ReadByte(const uint16_t InAddress) const
{
    if (InAddress >= 0x0000 && InAddress <= 0xFFFF)
    {
        return m_MemoryBuffer[InAddress];
    }

    std::cout << "Error: Segmentation fault! Attempting to read outside of memory bounds." << std::endl;
    __debugbreak();

    return 0;
}

void Memory::WriteByte(const uint8_t InData, const uint16_t InAddress)
{
    if (InAddress >= 0x00 && InAddress <= 0xFFFF)
    {
        m_MemoryBuffer[InAddress] = InData;
    }
}