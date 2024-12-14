#pragma once

class Memory;

/**
 * Represents the NES data bus.
 * Devices can be attached to the bus and independantly read/write to it
 */
class Bus
{
public:
    Bus(std::shared_ptr<Memory> InMemory);
    ~Bus() = default;

    /**
     * Writes the specified data to the address on the bus.
     * Different devices can be mapped to different addressess so the bus will determine which device receives this value.
     */
    void WriteData(const uint8_t InData, const uint16_t InAddress);

    /**
     * Reads the data stored at the specified address from the bus.
     * Different devices can be mapped to different addressess so the bus will determine which device we read this value from.
     */
    uint8_t ReadData(const uint16_t InAddress) const;

private:
    std::shared_ptr<Memory> m_pMemory = nullptr;
};