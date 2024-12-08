#include "pch.h"
#include "System/Memory.h"
#include "Core/Core.h"

Memory::Memory()
{
    for (uint16_t i = 0; i < k_MaxMemory; i++)
    {
        m_MemoryBuffer[i] = 0;
    }
}

int8_t Memory::ReadByte(const Registers& InRegisterState, const EAddressingMode InAddressingMode) const
{
    UNUSED_PARAMETER(InRegisterState);
    UNUSED_PARAMETER(InAddressingMode);

    return 0;
}

int8_t Memory::ReadByte(const uint16_t InAddress) const
{
    return m_MemoryBuffer[InAddress];
}

void Memory::WriteByte(const int8_t InData, const uint16_t InAddress)
{
    m_MemoryBuffer[InAddress] = InData;
}
