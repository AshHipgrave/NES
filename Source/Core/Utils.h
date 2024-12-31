#pragma once

struct OpCode;
struct CpuRegisters;

enum class ECpuFlag : uint8_t;

/**
 * Common utility functions that don't belong to a single component or class.
 */
namespace Utils
{
    /**
     * Tests if a specific bit is set in the provided value. Returns true if the bit is set, false otherwise.
     * For example: If the value 0x00000100 is provided and the position is 2 then this will return true.
     */
    template<typename T>
    bool IsBitSet(const T InValue, const int InPosition)
    {
        return (InValue & (1 << InPosition)) != 0;
    }

    /**
     * Converts the specified integer into a hexadecimal string. The output string will be padded according to the size of the type.
     * For example, a uint8_t of 255 will become FF, but for uint16_t it would become 00FF
     *
     * Note: uint8_t values are implicitly cast to a uint16_t because uint8_t is an alias for unsigned char, which doesn't work correctly with the formatting we want to use.
     */
    std::string ConvertToHexString(const uint16_t InInteger);

    /**
     * Returns the high byte of a 16-bit value.
     * For example: if 0xAAFF is provided this will return 0xAA
     */
    uint8_t GetHighByte(const uint16_t InValue);

    /**
     * Returns the lowbyte of a 16-bit value.
     * For example: if 0xAAFF is provided this will return 0xFF
     */
    uint8_t GetLowByte(const uint16_t InValue);

    /**
     * Combines the provided low and high bytes into a single 16-bit value (aka 'Word' size).
     * For example: If low = 0xFF and high = 0xAA then this will return 0xAAFF
     */
    uint16_t MakeWord(const uint8_t InLowByte, const uint8_t InHighByte);

    /**
     * Checks if a memory operation crossed a page boundry.
     * The 6502 CPU groups memory into 256 byte "pages". If an operation crosses the boundry between one page and another it can incur a performance penalty.
     */
    bool DidCrossPageBoundry(const uint16_t InStartAddress, const uint16_t InEndAddress);

    /**
     * Converts the specified flag name into its short-hand string representation.
     * For example: The 'Overflow' flag is returned as the string "V" (similar to how it would be shown in 6502 manuals/debuggers etc).
     */
    std::string ConvertFlagToString(const ECpuFlag InFlag);

    /**
     * Logs the specified CPU instruction for debugging purposes. The format is matched as closely as possible to the example log supplied with the nestest ROM.
     * 
     * Example output:
     * 
     *     C000  4C F5 C5  JMP $C5F5           A:00 X:00 Y:00 P:24 SP:FD PPU:  0, 21 CYC:7
     *     C5F5  A2 00     LDX #$00            A:00 X:00 Y:00 P:24 SP:FD PPU:  0, 30 CYC:10
     *     C5F7  86 00     STX $00 = 00        A:00 X:00 Y:00 P:26 SP:FD PPU:  0, 36 CYC:12
     * 
     * The format used is:
     *     'Program Counter' 'Instruction Memory' 'Decompiled Assembly Code' 'CPU Registers' 'PPU Scanline' 'PPU Cycle' 'Total CPU Cycles'
     */
    void LogInstruction(const OpCode InOpCode, const CpuRegisters InCpuRegisters, const uint64_t InTotalCycles);

    /**
     * Decompiles the specified CPU instruction into an assembly code string so that it can be printed to a log.
     */
    std::string DecompileInstruction(const OpCode InOpCode, const CpuRegisters InCpuRegisters);
}