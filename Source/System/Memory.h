#pragma once

struct Registers;
enum class EAddressingMode : uint8_t;

class Memory
{
public:
    Memory();

    uint8_t ReadViaAddressingMode(const Registers& InRegisterState, const EAddressingMode InAddressingMode, bool* bOutDidCrossPageBoundry = nullptr) const;

    uint8_t ReadByte(const uint16_t InAddress) const;
    void WriteByte(const uint8_t InData, const uint16_t InAddress);

private:
    static constexpr uint16_t k_MaxMemory = 65535;

    int8_t m_MemoryBuffer[k_MaxMemory];
};