#include "pch.h"
#include "System/PPU.h"
#include "System/Bus.h"
#include "Core/Core.h"
#include "Core/Utils.h"
#include "System/Cartridge.h"

PPU::PPU(Bus* InDataBus)
    : m_pDataBus(InDataBus)
{
    m_ScanLine = -1;
    m_CurrentCycle = 0;
    m_LastReadBuffer = 0;

    m_VRAM.resize(2048); // TODO: Make larger?
    m_OAMData.resize(256);
    m_PaletteTable.resize(32);

    m_Registers = PPURegisters();
}

PPU::~PPU()
{
    m_VRAM.clear();
    m_OAMData.clear();
    m_PaletteTable.clear();

    m_pCartridge = nullptr;
}

void PPU::Tick()
{
    m_CurrentCycle++;

    if (m_CurrentCycle >= 341)
    {
        m_CurrentCycle = 0;
        m_ScanLine++;

        if (m_ScanLine >= 261)
        {
            m_ScanLine = -1;

            m_pDataBus->NotifyFrameComplete();
        }
    }
}

void PPU::Draw()
{
    // TODO - Convert the NES framebuffer into an SDLTexture and draw to the screen.
}

void PPU::LoadCartridge(Cartridge* InCartridge)
{
    m_pCartridge = InCartridge;
}

void PPU::WriteData(const uint8_t InData, const uint16_t InAddress)
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
            m_OAMData[m_Registers.OAMAddress++] = InData;
            break;
        }
        case 0x0005:
        {
            m_Registers.Scroll = InData;
            break;
        }
        case 0x0006:
        {
            if (m_bWriteAddressHighBit)
            {
                m_Registers.PPUAddress = (m_Registers.PPUAddress & 0x00FF) | (InData << 8);
                m_bWriteAddressHighBit = false;
            }
            else
            {
                m_Registers.PPUAddress = (m_Registers.PPUAddress & 0x00FF) | InData;
                m_bWriteAddressHighBit = true;
            }
            break;
        }
        case 0x0007:
        {
            m_VRAM[m_Registers.PPUAddress] = InData;

            const bool bIsInVerticalIncrementMode = Utils::IsBitSet(m_Registers.Control, 2);
            const uint8_t incrementAmount = bIsInVerticalIncrementMode ? 32 : 1;

            m_Registers.PPUAddress += incrementAmount;

            break;
        }
        default:
        {
            std::cout << "ERROR: Attempted to write to an invalid PPU memory address" << std::endl;
            EMULATOR_DEBUG_BREAK();
        }
    }
}

uint8_t PPU::ReadData(const uint16_t InAddress)
{
    switch (InAddress)
    {
        case 0x0002:
        {
            return m_Registers.Status;
        }
        case 0x0004:
        {
            return m_Registers.OAMData;
        }
        case 0x0007:
        {
            // PPU reads are delayed by 1 cycle because the PPU needs to fetch it from external RAM.
            // The CPU will usually therefore request the data twice (the first time is a dummy read).
            const uint8_t lastRead = m_LastReadBuffer;

            m_LastReadBuffer = m_VRAM[m_Registers.PPUAddress];

            return lastRead;
        }
    }

    std::cout << "ERROR: Attempted to read from an invalid PPU memory address" << std::endl;

    EMULATOR_DEBUG_BREAK();

    return 0;
}

uint8_t PPU::InternalReadData(const uint16_t InAddress) const
{
    // TODO: Read from other memory sources? (VRAM, OAM etc).

    return m_pCartridge->ReadCharacterData(InAddress);
}