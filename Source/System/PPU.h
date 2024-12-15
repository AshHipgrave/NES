#pragma once

#include "Core/Types.h"

class PPU
{
public:
    PPU();
    ~PPU();

    void WriteData(const uint8_t InData, uint16_t InAddress);
    uint8_t ReadData(uint16_t InAddress);

private:
    uint8_t m_OAMBuffer[256];
    uint8_t m_PaletteBuffer[32];
    uint8_t m_VRAMBuffer[0x4000];

    uint32_t m_Framebuffer[256 * 240];

    PPURegisters m_Registers;

    // Tracks if we should write the PPUADDR high bit or low bit.
    // Only one 8-bit value is set at a time so the first time we write to the low bit and the second time to the high bit.
    bool bWriteAddressHighBit = false;
};