#pragma once

class Cpu;
class PPU;
class Memory;
class Cartridge;

/**
 * Represents the NES data bus.
 * Devices can be attached to the bus and independantly read/write to it
 */
class Bus
{
public:
    Bus();
    ~Bus();

    static Bus* Get();

    Cpu* GetCPU() const;
    Memory* GetMemory() const;
    Cartridge* GetCartridge() const;

    bool GetIsSingleStepEnabled() const;
    void SetSingleStepModeEnabled(const bool bInIsEnabled);
    void SetCanPerformSingleStep();

    /**
    * Initialises the devices connected to the bus
    */
    void InitDevices();

    /**
     * Loads the specified ROM file into program memory and begins executing it.
     */
    void LoadCartridge(const std::string& InFileName);

    /**
     * Executes a single clock cycle
     */
    void Tick();

    /**
     * Checks if we've loaded a ROM cartridge or not
     */
    bool HasCartridgeLoaded() const;

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
    // TODO: Make smart pointers:
    Cpu* m_pCpu = nullptr;
    PPU* m_pPPU = nullptr;
    Memory* m_pMemory = nullptr;
    Cartridge* m_pCartridge = nullptr;

    bool m_bEnableSingleStepMode = false;
    bool m_bCanSingleStep = false;

    static Bus* g_pBus;
};