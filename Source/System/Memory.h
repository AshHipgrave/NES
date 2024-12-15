#pragma once

class Memory
{
    friend class UIManager;

public:
    Memory();

    uint8_t ReadByte(const uint16_t InAddress) const;
    void WriteByte(const uint8_t InData, const uint16_t InAddress);

private:
    static constexpr uint16_t k_MaxMemory = 2048;

    int8_t m_MemoryBuffer[k_MaxMemory];
};