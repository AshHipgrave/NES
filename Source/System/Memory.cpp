#include "pch.h"
#include "System/Memory.h"
#include "Core/Core.h"
#include "Logging/Log.h"

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

    EMULATOR_LOG_FATAL("Segmentation fault! Attempting to read outside of memory bounds.");
}

void Memory::WriteByte(const uint8_t InData, const uint16_t InAddress)
{
    if (InAddress >= 0 && InAddress <= k_MaxMemory)
    {
        m_MemoryBuffer[InAddress] = InData;
    }
    else
    {
        EMULATOR_LOG_FATAL("Segmentation fault! Attempting to write outside of memory bounds.");
    }
}