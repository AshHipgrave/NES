#include "pch.h"
#include "System/Memory.h"
#include "Core/Core.h"

Memory::Memory()
{
    for (uint16_t i = 0; i < k_MaxMemory; i++)
    {
        // TODO: The NES doesn't zero-initialise RAM and leaves it containing random/junk data:
        m_MemoryBuffer[i] = 0;
    }
}

uint8_t Memory::ReadByte(const uint16_t InAddress) const
{
    if (InAddress >= 0 && InAddress <= k_MaxMemory)
    {
        return m_MemoryBuffer[InAddress];
    }

    std::cout << "Error: Segmentation fault! Attempting to read outside of memory bounds." << std::endl;

    EMULATOR_DEBUG_BREAK();

    return 0;
}

void Memory::WriteByte(const uint8_t InData, const uint16_t InAddress)
{
    if (InAddress >= 0 && InAddress <= k_MaxMemory)
    {
        m_MemoryBuffer[InAddress] = InData;
    }
}