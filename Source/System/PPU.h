#pragma once

#include "Types/PPURegisters.h"

class Bus;
class Cartridge;

enum class EMirrorMode : uint8_t;

class PPU
{
    friend class UIManager;

public:
    PPU(Bus* InDataBus);
    ~PPU();

    void Tick();
    void Draw();

    void LoadCartridge(Cartridge* InCartridge);

    void WriteData(const uint8_t InData, const uint16_t InAddress);
    uint8_t ReadData(const uint16_t InAddress);

    int16_t GetCurrentScanLine() const;
    uint16_t GetCurrentCycle() const;

private:
    uint8_t InternalReadData(const uint16_t InAddress);
    void InternalWriteData(const uint8_t InData, const uint16_t InAddress);
    
    void ConfigureMirroring(const EMirrorMode InMirroringMode);

private:
    int16_t m_ScanLine;
    uint16_t m_CurrentCycle;

    uint8_t m_LastReadBuffer;

    PPURegisters m_Registers;

    std::array<uint8_t, 256> m_OAMData;
    std::array<uint8_t, 32> m_PaletteTable;

    std::array<uint8_t, 2048> m_NametableVRAM;
    std::array<uint8_t*, 4> m_pNametablePointers;

    Bus* m_pDataBus = nullptr;
    Cartridge* m_pCartridge = nullptr;

    // Tracks if we should access the PPUADDR high bit or low bit.
    // We can only access 8-bits per-read/write so the first time we access the high bit and the second time the low bit.
    bool m_bWriteAddressHighBit = true;
};