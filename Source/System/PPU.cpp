#include "pch.h"
#include "System/PPU.h"
#include "Core/Core.h"
#include "System/Bus.h"
#include "Core/Utils.h"
#include "Logging/Log.h"
#include "Enums/PPUFlags.h"
#include "Enums/MirrorMode.h"
#include "System/Cartridge.h"

PPU::PPU(Bus* InDataBus)
    : m_pDataBus(InDataBus)
{
    m_ScanLine = 0;
    m_CurrentCycle = 0;
    m_LastReadBuffer = 0;

    m_NametableVRAM.fill(0);
    m_OAMData.fill(0);
    m_PaletteTable.fill(0);
    m_pNametablePointers.fill(0);

    for (int i = 0; i < 4; i++)
    {
        m_pNametablePointers[i] = nullptr;
    }

    m_Registers = PPURegisters();

    m_Registers.SetFlag(m_Registers.Status, EPPUStatusFlag::VBlank, true);
    m_Registers.SetFlag(m_Registers.Status, EPPUStatusFlag::SpriteOverflow, true);

    m_bEvenOddFlag = false;
    m_bWriteLatchStatus = false;
}

PPU::~PPU()
{
    m_pCartridge = nullptr;
}

void PPU::Reset()
{
    m_ScanLine = 0;
    m_CurrentCycle = 0;
    m_LastReadBuffer = 0;

    m_Registers.Mask = 0;
    m_Registers.Scroll = 0;
    m_Registers.Control = 0;

    m_bEvenOddFlag = false;
    m_bWriteLatchStatus = false;
}

void PPU::Tick()
{
    m_bEvenOddFlag = !m_bEvenOddFlag;
    m_CurrentCycle++;

    // TODO: Fetch the pixel to draw from VRAM.

    if (m_CurrentCycle >= 341)
    {
        m_ScanLine++;

        if (m_ScanLine == 241)
        {
            m_Registers.SetFlag(m_Registers.Status, EPPUStatusFlag::VBlank, true);

            if (m_Registers.IsFlagSet(m_Registers.Control, EPPUControlFlags::VBlankNMIEnable))
            {
                m_pDataBus->NotifyVBlank();
            }
        }
        else if (m_ScanLine == 261)
        {
            // TODO: Shift registers
        }
        else if (m_ScanLine == 262)
        {
            // TODO: Handle scrolling

            m_ScanLine = 0;
            m_CurrentCycle = 0;

            m_Registers.SetFlag(m_Registers.Status, EPPUStatusFlag::VBlank, false);

            m_pDataBus->NotifyFrameComplete();
        }
    }
}

void PPU::Draw()
{
    // TODO - Convert the NES framebuffer into an SDL_Texture and draw to the screen.
}

void PPU::LoadCartridge(Cartridge* InCartridge)
{
    Reset();

    m_pCartridge = InCartridge;

    ConfigureMirroring(InCartridge->GetMirrorMode());
}

void PPU::WriteData(const uint8_t InData, const uint16_t InAddress)
{
    switch (InAddress)
    {
        case 0x0000:
        {
            const bool bWasNMIEnabledBefore = m_Registers.IsFlagSet(m_Registers.Control, EPPUControlFlags::VBlankNMIEnable);

            m_Registers.Control = InData;

            const bool bIsNMIEnabledAfter = m_Registers.IsFlagSet(m_Registers.Control, EPPUControlFlags::VBlankNMIEnable);
            const bool bIsInVBlank = m_Registers.IsFlagSet(m_Registers.Status, EPPUStatusFlag::VBlank);

            // NMIEnable was changed from disabled to enabled during VBlank. Doing so should trigger an NMI immediately
            if (bWasNMIEnabledBefore == false && bIsNMIEnabledAfter == true && bIsInVBlank == true)
            {
                m_pDataBus->NotifyVBlank();
            }
            break;
        }
        case 0x0001:
        {
            m_Registers.Mask = InData;
            break;
        }
        case 0x0003:
        {
            m_Registers.OAMAddress = InData;
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
            m_bWriteLatchStatus = !m_bWriteLatchStatus;

            break;
        }
        case 0x0006:
        {
            if (m_bWriteLatchStatus)
            {
                m_Registers.PPUAddress = (m_Registers.PPUAddress & 0x00FF) | (InData << 8);
                m_bWriteLatchStatus = false;
            }
            else
            {
                m_Registers.PPUAddress = (m_Registers.PPUAddress & 0x00FF) | InData;
                m_bWriteLatchStatus = true;
            }
            break;
        }
        case 0x0007:
        {
            InternalWriteData(InData, m_Registers.PPUAddress);

            const bool bIsInVerticalIncrementMode = m_Registers.IsFlagSet(m_Registers.Control, EPPUControlFlags::VRAMIncrement);
            const uint8_t incrementAmount = bIsInVerticalIncrementMode ? 32 : 1;

            m_Registers.PPUAddress += incrementAmount;

            break;
        }
        default:
        {
            EMULATOR_LOG_FATAL("Segmentation Fault! Attempted to write to an invalid PPU memory address.");
        }
    }
}

uint8_t PPU::ReadData(const uint16_t InAddress)
{
    switch (InAddress)
    {
        case 0x0002:
        {
            const uint8_t result = (m_Registers.GetStatusFlags() & 0xE0) | (m_LastReadBuffer & 0x1F);

            m_Registers.SetFlag(m_Registers.Status, EPPUStatusFlag::VBlank, false);
            m_bWriteLatchStatus = false;

            return result;
        }
        case 0x0004:
        {
            return m_OAMData[m_Registers.OAMAddress];
        }
        case 0x0007:
        {
            const uint8_t data = InternalReadData(m_Registers.PPUAddress);

            const bool bIsInVerticalIncrementMode = m_Registers.IsFlagSet(m_Registers.Control, EPPUControlFlags::VRAMIncrement);
            const uint8_t incrementAmount = bIsInVerticalIncrementMode ? 32 : 1;

            m_Registers.PPUAddress += incrementAmount;

            return data;
        }
    }

    EMULATOR_LOG_FATAL("Segmentation Fault! Attempted to read from an invalid PPU memory address.");
}

int16_t PPU::GetCurrentScanLine() const
{
    return m_ScanLine;
}

uint16_t PPU::GetCurrentCycle() const
{
    return m_CurrentCycle;
}

uint8_t PPU::InternalReadData(const uint16_t InAddress)
{
    if (InAddress >= 0x0000 && InAddress <= 0x1FFF)
    {
        return m_pCartridge->ReadCharacterData(InAddress);
    }
    else if (InAddress >= 0x2000 && InAddress <= 0x2FFF)
    {
        const uint16_t index = (InAddress - 0x2000) % 0x1000;
        const uint16_t nametableIndex = index / 0x0400;
        const uint16_t offset = index % 0x0400;

        // PPU reads are delayed by 1 cycle because the PPU needs to fetch it from external RAM.
        // The CPU will usually therefore request the data twice (the first time is a dummy read).
        const uint8_t lastRead = m_LastReadBuffer;

        m_LastReadBuffer = m_pNametablePointers[nametableIndex][offset];

        return lastRead;
    }
    else if (InAddress >= 0x3F00 && InAddress <= 0x3FFF)
    {
        return m_PaletteTable[InAddress - 0x3F00];
    }
    else
    {

        EMULATOR_LOG_FATAL("Segmentation Fault! PPU attempted to read from an invalid memory address.");
    }
}

void PPU::InternalWriteData(const uint8_t InData, const uint16_t InAddress)
{
    // TODO: Support additional write locations (currently this is only called for VRAM/Nametable writes).

    const uint16_t index = (InAddress - 0x2000) % 0x1000;
    const uint16_t nametableIndex = index / 0x0400;
    const uint16_t offset = index % 0x0400;

    m_pNametablePointers[nametableIndex][offset] = InData;
}

void PPU::ConfigureMirroring(const EMirrorMode InMirroringMode)
{
    switch (InMirroringMode)
    {
        case EMirrorMode::Horizontal:
            m_pNametablePointers[0] = m_NametableVRAM.data();
            m_pNametablePointers[1] = m_NametableVRAM.data();
            m_pNametablePointers[2] = m_NametableVRAM.data() + 0x0400;
            m_pNametablePointers[3] = m_NametableVRAM.data() + 0x0400;
            break;

        case EMirrorMode::Vertical:
            m_pNametablePointers[0] = m_NametableVRAM.data();
            m_pNametablePointers[1] = m_NametableVRAM.data() + 0x0400;
            m_pNametablePointers[2] = m_NametableVRAM.data();
            m_pNametablePointers[3] = m_NametableVRAM.data() + 0x0400;
            break;

        case EMirrorMode::FourScreen:
        case EMirrorMode::SingleScreen:
        default:
            // TODO.
            EMULATOR_DEBUG_BREAK();
            break;
    }
}
