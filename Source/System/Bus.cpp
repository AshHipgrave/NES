#include "pch.h"
#include "Core/Core.h"
#include "System/Bus.h"
#include "System/Cpu.h"
#include "System/PPU.h"
#include "Logging/Log.h"
#include "System/Memory.h"
#include "System/Cartridge.h"
#include "Enums/InterruptType.h"

Bus* Bus::g_pBus = nullptr;

Bus::Bus()
    : m_pPPU(nullptr)
    , m_pCpu(nullptr)
    , m_pMemory(nullptr)
    , m_pCartridge(nullptr)
{
    g_pBus = this;
}

Bus::~Bus()
{
    // TODO: Use smart pointers...

    delete m_pMemory;
    delete m_pPPU;
    delete m_pCpu;

    if (m_pCartridge)
        delete m_pCartridge;

    m_pMemory = nullptr;
    m_pPPU = nullptr;
    m_pCpu = nullptr;
}

Bus* Bus::Get()
{
    return g_pBus;
}

void Bus::InitDevices()
{
    m_pCpu = new Cpu(this);
    m_pPPU = new PPU(this);
    m_pMemory = new Memory();
}

void Bus::LoadCartridge(const std::string& InFileName)
{
    m_pCartridge = Cartridge::LoadFromFile(InFileName);
    m_pPPU->LoadCartridge(m_pCartridge);

    m_pCpu->Reset();
}

void Bus::Tick()
{
    if (m_bEnableSingleStepMode == true && m_bCanSingleStep == false)
        return;

    const uint8_t cpuCycles = m_pCpu->Tick();

    for (int i = 0; i < cpuCycles * 3; i++)
    {
        m_pPPU->Tick();
    }

    m_bCanSingleStep = false;
}

bool Bus::HasCartridgeLoaded() const
{
    return m_pCartridge != nullptr;
}

void Bus::NotifyVBlank()
{
    m_pCpu->HandleInterrupt(EInterruptType::NMI);
}

void Bus::NotifyFrameComplete()
{
    // TODO ?!
}

void Bus::WriteData(const uint8_t InData, const uint16_t InAddress)
{
    if (InAddress >= 0x0000 && InAddress <= 0x1FFF)
    {
        m_pMemory->WriteByte(InData, (InAddress & 0x07FF));
    }
    else if (InAddress >= 0x2000 && InAddress <= 0x3FFF)
    {
        m_pPPU->WriteData(InData, (InAddress & 0x0007));
    }
    else if ((InAddress >= 0x4000 && InAddress <= 0x4013) || InAddress == 0x4015 || InAddress == 0x4017)
    {
        EMULATOR_LOG_DEBUG("Attempting to write to APU registers. This has not been implemented yet!");
    }
    else if (InAddress == 0x4014)
    {
        EMULATOR_LOG_DEBUG("Attempting to write to DMA address. This has not been implemented yet!");
    }
    else if (InAddress >= 0x4016 && InAddress <= 0x4017)
    {
        EMULATOR_LOG_DEBUG("Attempting to write to I/O registers. This has not been implemented yet!");
    }
    else if (InAddress >= 0x4018 && InAddress <= 0x401F)
    {
        EMULATOR_LOG_WARN("Attempting to write into CPU Test Mode memory region (4018 - 401F). This is not allowed!");
    }
    else if (InAddress >= 0x6000 && InAddress <= 0xFFFF)
    {
        EMULATOR_LOG_DEBUG("Attempting to write into cartridge memory. This has not been implemented yet!");
    }
    else
    {
        EMULATOR_LOG_FATAL("Segmentation fault! Attempting to write outside of bounds.");
    }
}

uint8_t Bus::ReadData(const uint16_t InAddress) const
{
    if (InAddress >= 0x0000 && InAddress <= 0x1FFF)
    {
        return m_pMemory->ReadByte((InAddress & 0x1FFF));
    }
    else if (InAddress >= 0x2000 && InAddress <= 0x3FFF)
    {
        return m_pPPU->ReadData((InAddress & 0x0007));
    }
    else if ((InAddress >= 0x4000 && InAddress <= 0x4013) || InAddress == 0x4015 || InAddress == 0x4017)
    {
        EMULATOR_LOG_DEBUG("Attempting to read from APU registers. This has not been implemented yet!");

        return 0xFF;
    }
    else if (InAddress == 0x4014)
    {
        EMULATOR_LOG_DEBUG("Attempting to read from DMA address. This has not been implemented yet!");

        return 0xFF;
    }
    else if (InAddress >= 0x4016 && InAddress <= 0x4017)
    {
        EMULATOR_LOG_DEBUG("Attempting to read from I/O registers. This has not been implemented yet!");

        return 0xFF;
    }
    else if (InAddress >= 0x4018 && InAddress <= 0x401F)
    {
        EMULATOR_LOG_WARN("Attempting to read from CPU Test Mode memory region (4018 - 401F). This is not allowed!");

        return 0xFF;
    }
    else if (InAddress >= 0x6000 && InAddress <= 0xFFFF)
    {
        return m_pCartridge->ReadProgramData(InAddress & 0x3FFF);
    }
    else
    {
        EMULATOR_LOG_FATAL("Segmentation fault! Attempting to read outside of bounds.");
    }
}

Cpu* Bus::GetCPU() const
{
    return m_pCpu;
}

PPU* Bus::GetPPU() const
{
    return m_pPPU;
}

Memory* Bus::GetMemory() const
{
    return m_pMemory;
}

Cartridge* Bus::GetCartridge() const
{
    return m_pCartridge;
}

bool Bus::GetIsSingleStepEnabled() const
{
    return m_bEnableSingleStepMode;
}

void Bus::SetSingleStepModeEnabled(const bool bInIsEnabled)
{
    m_bEnableSingleStepMode = bInIsEnabled;
}

void Bus::SetCanPerformSingleStep()
{
    m_bCanSingleStep = true;
}
