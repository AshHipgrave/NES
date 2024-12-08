#pragma once

struct Registers;
enum class EAddressingMode : uint8_t;

class Memory
{
public:
    Memory();

    int8_t ReadByte(const Registers& InRegisterState, const EAddressingMode InAddressingMode) const;
    int8_t ReadByte(const uint16_t InAddress) const;

    void WriteByte(const int8_t InData, const uint16_t InAddress);

private:
    static constexpr uint16_t k_MaxMemory = 2048;

    int8_t m_MemoryBuffer[k_MaxMemory];
};