#pragma once

class Memory;

class Bus
{
public:
    Bus(std::shared_ptr<Memory> InMemory);
    ~Bus() = default;

    void WriteData(const uint8_t InData, const uint16_t InAddress);
    uint8_t ReadData(const uint16_t InAddress) const;

private:
    std::shared_ptr<Memory> m_pMemory = nullptr;
};