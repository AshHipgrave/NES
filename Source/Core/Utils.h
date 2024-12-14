#pragma once

/**
 * Common utility functions that don't belong to a single component.
 */
namespace Utils
{
    /**
     * Tests if the specified bit is set.
     * For example: If the value 0x00010000 is provided and the position is 4 this will return true.
     */
    template<typename T>
    bool IsBitSet(const T InValue, const int InPosition)
    {
        return (InValue & (1 << InPosition)) != 0;
    }

    /**
     * Returns the highbyte of a 16-bit value.
     * For example: if 0xAAFF is provided this will return 0xAA
     */
    uint8_t GetHighByte(const uint16_t InValue);


    /**
     * Returns the lowbyte of a 16-bit value.
     * For example: if 0xAAFF is provided this will return 0xFF
     */
    uint8_t GetLowByte(const uint16_t InValue);

    /**
     * Combines the provided low and highbytes into a single 16-bit value.
     * For example: If low = 0xAA and high=0xFF this will return 0xFFAA
     */
    uint16_t MakeDword(const uint8_t InLowByte, const uint8_t InHighByte);

    /**
     * Checks if the specified memory operation crossed a page boundry.
     * The 6502 groups memory into 256 byte "pages". If an operation crosses the boundry between one page and another it can incur a performance penalty
     */
    bool DidCrossPageBoundry(const uint16_t InStartAddress, const uint16_t InEndAddress);
}