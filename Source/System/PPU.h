#pragma once

#include "Core/Types.h"

class Bus;
class Cartridge;

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
    uint8_t InternalReadData(const uint16_t InAddress) const;

private:
    int16_t m_ScanLine;
    uint16_t m_CurrentCycle;

    uint8_t m_LastReadBuffer;

    PPURegisters m_Registers;

    std::vector<uint8_t> m_VRAM; //aka nametables
    std::vector<uint8_t> m_OAMData;
    std::vector<uint8_t> m_PaletteTable;

    Bus* m_pDataBus = nullptr;
    Cartridge* m_pCartridge = nullptr;

    // Tracks if we should write the PPUADDR high bit or low bit.
    // We can only set 8-bits at once so the first time we set the high bit and the second time the low bit.
    bool m_bWriteAddressHighBit = true;
};