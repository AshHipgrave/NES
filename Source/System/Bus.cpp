#include "pch.h"
#include "System/Bus.h"
#include "System/PPU.h"
#include "System/Memory.h"
#include "System/Cartridge.h"
#include "CPU/6502.h"
#include "Core/Core.h"

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
    m_pCpu->NMI();
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
        // TODO: APU hasn't been implemented.
    }
    else if (InAddress >= 0x4016 && InAddress <= 0x4017)
    {
        // TODO: Controller hasn't been implemented.
    }
    else if (InAddress >= 0x8000 && InAddress <= 0xFFFF)
    {
        // TODO: Writing to cartridge memory is not yet supported. If we reach this we've likely loaded a ROM which relies on mappers (?)
        EMULATOR_DEBUG_BREAK();
    }
    else
    {
        std::cout << "Error: Segmentation fault! Attempting to write outside of bounds." << std::endl;

        EMULATOR_DEBUG_BREAK();
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
    else if (InAddress >= 0x4016 && InAddress <= 0x4017)
    {
        // TODO: Controller hasn't been implemented.
        return 0;
    }
    else if (InAddress >= 0x8000 && InAddress <= 0xFFFF)
    {
        return m_pCartridge->ReadProgramData(InAddress & 0x3FFF);
    }
    else
    {
        std::cout << "Error: Segmentation fault! Attempting to read outside of bounds." << std::endl;

        EMULATOR_DEBUG_BREAK();
    }

    return 0;
}
