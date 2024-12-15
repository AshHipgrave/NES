#include "pch.h"
#include "System/PPU.h"
#include "Core/Core.h"
#include "Core/Utils.h"

PPU::PPU()
{
    m_Registers = PPURegisters();
}

PPU::~PPU()
{

}

void PPU::WriteData(const uint8_t InData, uint16_t InAddress)
{
    switch (InAddress)
    {
        case 0x0000:
        {
            m_Registers.Control = InData;
            break;
        }
        case 0x0001:
        {
            m_Registers.Mask = InData;
            break;
        }
        case 0x0002:
        {
            m_Registers.OAMAddress = InData;
            break;
        }
        case 0x0003:
        {
            m_Registers.OAMData = InData;
            break;
        }
        case 0x0004:
        {
            m_OAMBuffer[m_Registers.OAMAddress++] = InData;
            break;
        }
        case 0x0005:
        {
            m_Registers.Scroll = InData;
            break;
        }
        case 0x0006:
        {
            if (bWriteAddressHighBit)
            {
                m_Registers.PPUAddress = (m_Registers.PPUAddress & 0x00FF) | (InData << 8);
                bWriteAddressHighBit = false;
            }
            else
            {
                m_Registers.PPUAddress = (m_Registers.PPUAddress & 0x00FF) | InData;
                bWriteAddressHighBit = true;
            }
            break;
        }
        case 0x0007:
        {
            m_VRAMBuffer[m_Registers.PPUAddress++] = InData;
            break;
        }
    }
}

uint8_t PPU::ReadData(uint16_t InAddress)
{
    // TODO:
    return m_VRAMBuffer[InAddress];
}