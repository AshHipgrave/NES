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
    PPU* GetPPU() const;
    Memory* GetMemory() const;
    Cartridge* GetCartridge() const;

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
     * Called by the PPU to trigger an NMI on the next CPU cycle
     */
    void NotifyFrameComplete();

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

    /**
    * [Debug] Returns true if the emulator is in single-step mode (only executes 1 opcode before waiting).
    * Designed to allow for debugging the execution of a single instruction to confirm it's executed and set the emulator state as expected.
    */
    bool GetIsSingleStepEnabled() const;

    /**
     * [Debug] Enables (or disables) single-step mode. When enabled the emulator will not execute any instructions until prompted via the F5 key.
     * Once prompted it will only execute a single instruction before pausing and waiting for the next prompt. When disabled the emulator will run normally.
     */
    void SetSingleStepModeEnabled(const bool bInIsEnabled);

    /**
     * [Debug] Tells the emualtor it is allowed to execute a single instruction (i.e. the user has pressed F5 to perform a single-step).
     * The flag is reset after the step has been performed to re-enter the waiting state.
     */
    void SetCanPerformSingleStep();

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