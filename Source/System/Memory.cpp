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

uint16_t Memory::GetAddress(const Registers& InRegisterState, const EAddressingMode InAddressingMode, bool* bOutDidCrossPageBoundry /*= nullptr*/) const
{
    switch (InAddressingMode)
    {
        case EAddressingMode::Absolute:
        {
            const uint8_t lowbyte = m_MemoryBuffer[InRegisterState.ProgramCounter + 1];
            const uint8_t highbyte = ReadByte(InRegisterState.ProgramCounter + 2);

            const uint16_t address = Utils::Combine(lowbyte, highbyte);

            return address;
        }
        case EAddressingMode::AbsoluteX:
        {
            const uint8_t lowbyte = ReadByte(InRegisterState.ProgramCounter + 1);
            const uint8_t highbyte = ReadByte(InRegisterState.ProgramCounter + 2);

            uint16_t address = Utils::Combine(lowbyte, highbyte);
            address += InRegisterState.X;

            if (bOutDidCrossPageBoundry != nullptr)
            {
                *bOutDidCrossPageBoundry = Utils::DidCrossPageBoundry(InRegisterState.ProgramCounter, address);
            }

            return address;
        }
        case EAddressingMode::AbsoluteY:
        {
            const uint8_t lowbyte = ReadByte(InRegisterState.ProgramCounter + 1);
            const uint8_t highbyte = ReadByte(InRegisterState.ProgramCounter + 2);

            uint16_t address = Utils::Combine(lowbyte, highbyte);
            address += InRegisterState.Y;

            if (bOutDidCrossPageBoundry != nullptr)
            {
                *bOutDidCrossPageBoundry = Utils::DidCrossPageBoundry(InRegisterState.ProgramCounter, address);
            }

            return address;
        }
        case EAddressingMode::Immediate:
        {
            return InRegisterState.ProgramCounter + 1;
        }
        case EAddressingMode::Indirect:
        {
            const uint8_t pointerLow = ReadByte(InRegisterState.ProgramCounter + 1);
            const uint8_t pointerHigh = ReadByte(InRegisterState.ProgramCounter + 2);

            const uint16_t pointer = Utils::Combine(pointerLow, pointerHigh);

            const uint8_t lowbyte = ReadByte(pointer);

            const uint16_t highbyteAddress = (pointer & 0xFF) == 0xFF ? (pointer & 0xFF00) : (pointer + 1);
            const uint8_t highbyte = ReadByte(highbyteAddress);

            const uint16_t address = Utils::Combine(lowbyte, highbyte);

            return address;
        }
        case EAddressingMode::IndirectX:
        {
            uint8_t effectiveZP = ReadByte(InRegisterState.ProgramCounter + 1);
            effectiveZP = (effectiveZP + InRegisterState.X) & 0xFF;

            const uint8_t lowbyte = ReadByte(effectiveZP);
            const uint8_t highbyte = ReadByte((effectiveZP + 1) & 0xFF);

            const uint16_t address = Utils::Combine(lowbyte, highbyte);

            return address;
        }
        case EAddressingMode::IndirectY:
        {
            const uint8_t zpAddress = ReadByte(InRegisterState.ProgramCounter + 1);

            const uint8_t lowbyte = ReadByte(zpAddress);
            const uint8_t highbyte = ReadByte((zpAddress + 1) & 0xFF);

            const uint16_t baseAddress = Utils::Combine(lowbyte, highbyte);
            const uint16_t address = baseAddress + InRegisterState.Y;

            if (bOutDidCrossPageBoundry != nullptr)
            {
                *bOutDidCrossPageBoundry = Utils::DidCrossPageBoundry(InRegisterState.ProgramCounter, address);
            }

            return address;
        }
        case EAddressingMode::Relative:
        {
            int8_t offset = ReadByte(InRegisterState.ProgramCounter + 1);

            const uint16_t finalAddress = InRegisterState.ProgramCounter + offset;

            if (bOutDidCrossPageBoundry != nullptr)
            {
                *bOutDidCrossPageBoundry = Utils::DidCrossPageBoundry(InRegisterState.ProgramCounter, finalAddress);
            }

            return finalAddress;
        }
        case EAddressingMode::ZeroPage:
        {
            return ReadByte(InRegisterState.ProgramCounter + 1);
        }
        case EAddressingMode::ZeroPageX:
        {
            const uint8_t baseAddress = ReadByte(InRegisterState.ProgramCounter + 1);
            const uint16_t address = (baseAddress + InRegisterState.X) & 0xFF;

            return address;
        }
        case EAddressingMode::ZeroPageY:
        {
            const uint8_t baseAddress = ReadByte(InRegisterState.ProgramCounter + 1);
            const uint16_t address = (baseAddress + InRegisterState.Y) & 0xFF;

            return address;
        }
    }

    std::cout << "Error: Unimplemented address mode" << std::endl;

    __debugbreak();
    return 0;
}

uint8_t Memory::ReadViaAddressingMode(const Registers& InRegisterState, const EAddressingMode InAddressingMode, bool* bOutDidCrossPageBoundry /* = nullptr */) const
{
    return ReadByte(
        GetAddress(InRegisterState, InAddressingMode, bOutDidCrossPageBoundry));
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