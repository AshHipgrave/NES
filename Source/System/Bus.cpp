#include "pch.h"
#include "System/Bus.h"
#include "System/Memory.h"
#include "Core/Core.h"

Bus::Bus(std::shared_ptr<Memory> InMemory)
    : m_pMemory(InMemory)
{
}

void Bus::WriteData(const uint8_t InData, const uint16_t InAddress)
{
    if (InAddress >= 0x0000 && InAddress <= 0x07FF)
    {
        m_pMemory->WriteByte(InData, InAddress);
        return;
    }

    // TEMP - Catch invalid memory reads (these probably are intended for cartridge ram or some other mapped device)
    EMULATOR_DEBUG_BREAK();
}

uint8_t Bus::ReadData(const uint16_t InAddress) const
{
    if (InAddress >= 0x0000 && InAddress <= 0x07FF)
    {
        return m_pMemory->ReadByte(InAddress);
    }

    // TEMP - Catch invalid memory reads (these probably are intended for cartridge ram or some other mapped device)
    EMULATOR_DEBUG_BREAK();

    return 0;
}
