#pragma once

struct OpCode;
struct CpuRegisters;

enum class ECpuFlag : uint8_t;

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
     * Converts the specified integer into a hexadecimal string. 
     * The output string will be padded according to the size of the type.
     * For example, a uint8_t of 255 will become FF, but for uint16_t it would become 00FF
     */
    std::string ConvertToHex(const uint16_t InInteger);

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
     * Combines the provided low and high bytes into a single 16-bit value.
     * For example: If low = 0xFF and high=0xAA this will return 0xAAFF
     */
    uint16_t MakeDword(const uint8_t InLowByte, const uint8_t InHighByte);

    /**
     * Checks if the specified memory operation crossed a page boundry.
     * The 6502 CPU groups memory into 256 byte "pages". If an operation crosses the boundry between one page and another it can incur a performance penalty
     */
    bool DidCrossPageBoundry(const uint16_t InStartAddress, const uint16_t InEndAddress);

    /**
     * Converts the specified flag enum into a string. The string is representative of how it would appear in 6502 manuals.
     * For example: The 'Overflow' flag enum is returned as the string "V" as this is how it appears in the CPU manual.
     */
    std::string ConvertFlagToString(const ECpuFlag InFlag);

    /**
     * Converts the specified opcode to a string matching its assembly instruction representation.
     * For example: If the opcode 0x8E is specified then this will return "STX".
     */
    std::string ConvertOpcodeToString(const uint8_t InOpcode);

    /**
     * Decompiles and logs the specified CPU instruction for debugging purposes. The format is matched as closely as possible to the nestest ROMs example log.
     * Example output:
     * 
     *     C000  4C F5 C5  JMP $C5F5           A:00 X:00 Y:00 P:24 SP:FD PPU:  0, 21 CYC:7
     *     C5F5  A2 00     LDX #$00            A:00 X:00 Y:00 P:24 SP:FD PPU:  0, 30 CYC:10
     *     C5F7  86 00     STX $00 = 00        A:00 X:00 Y:00 P:26 SP:FD PPU:  0, 36 CYC:12
     */
    std::string LogInstruction(const OpCode InOpCode, const CpuRegisters CpuStateBefore, const uint64_t InTotalCycles);
}