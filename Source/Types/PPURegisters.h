#pragma once

/**
 * Represents the memory mapped registers used by the PPU.
 */
struct PPURegisters
{
    /**
     * 8-bit control register which contains a mix of settings related to rendering.
     * This register is read-only to the CPU.
     */
    std::bitset<8> Control = 0;

    /**
     * 8-bit register which controls the rendering of sprites and backgrounds, as well as colour effects.
     */
    std::bitset<8> Mask = 0;

    /**
     * 8-bit status register which reflects the state of rendering related events and is primarily used for timing.
     * Only the first four bits are used, the remaining 4 bits are junk (usually the last data held on the data bus).
     */
    std::bitset<8> Status = 0;

    /**
     * 8-bit register which holds to address of the OAM (Object Attribute Memory) being accessed.
     * Note: Most games write 0x00 here and use the OAM Data register instead.
     */
    uint8_t OAMAddress = 0;

    /**
     * 8-bit register used for writing OAM (Object Attribute Memory) to.
     */
    uint8_t OAMData = 0;

    /**
     * 8-bit register used to change the scroll position.
     */
    uint8_t Scroll = 0;

    /**
     * 16-bit register used to facilitate communication between the CPU and PPU busses.
     */
    uint16_t PPUAddress = 0;

    /**
     * 8-bit CPU register that suspends the CPU so it can quickly copy a page of CPU memory to PPU OAM using DMA
     */
    uint8_t OAMDMA = 0;

    /**
     * Returns the current PPUMASK flags
     */
    uint8_t GetMaskFlags() const;

    /**
     * Returns the current PPUSTATUS flags
     */
    uint8_t GetStatusFlags() const;

    /**
     * Returns the current PPUCTRL flags
     */
    uint8_t GetControlFlags() const;

    /**
     * Tests if the specified flag is set for the provided register.
     */
    template<typename FlagEnum>
    bool IsFlagSet(const std::bitset<8> InRegister, const FlagEnum InFlag)
    {
        return InRegister.test(std::size_t(InFlag));
    }

    /**
     * Sets or clears the specified flag is set for the provided register.
     */
    template<typename FlagEnum>
    void SetFlag(std::bitset<8>& InOutRegister, const FlagEnum InFlag, const bool bInShouldSet)
    {
        InOutRegister.set(std::size_t(InFlag), bInShouldSet);
    }
};