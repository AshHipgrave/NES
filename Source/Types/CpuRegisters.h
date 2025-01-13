#pragma once

enum class ECpuFlag : uint8_t;

/**
 * Represents the registers available in a 6502 CPU.
 */
struct CpuRegisters
{
    /**
     * 16-bit register which points to the next instruction to be executed.
     */
    uint16_t ProgramCounter = 0;

    /**
     * 8-bit register that holds the low 8-bits of the next free location on the stack.
     */
    uint8_t StackPointer = 0;

    /**
     * 8-bit register used for all arithmetic and logical operations (with the exception of increments and decrements).
     */
    uint8_t Accumulator = 0;

    /**
     * 8-bit index register most commonly used to hold counters or offsets for accessing memory.
     * Also has a special function where it can get a copy of the stack pointer or change its value.
     */
    uint8_t X = 0;

    /**
     * Similar to the X register in that it is available for holding counter or offsets memory access and supports the same set of memory load, save and compare operations as wells as increments and decrements.
     * Unlike X however it has no special functions.
     */
    uint8_t Y = 0;

    /**
     * Represents a set of CPU status flags that are set or cleared as instructions execute to record the result of the operation.
     * The flags themselves are encapsulated by the 'ECpuFlag' enum.
     */
    std::bitset<8> Flags = 0;

    /**
     * Returns the CPU flags as an 8-bit number.
     * For example: If the flags are 0b00100100 then this will return decimal 36
     */
    uint8_t GetFlags() const;

    /**
     * Sets or clears the specified CPU flag
     */
    void SetFlag(const ECpuFlag InFlag, const bool bInShouldSet);

    /**
     * Checks if the specified flag is set or not
     */
    bool IsFlagSet(const ECpuFlag InFlag) const;

    /**
     * Resets the flags to the default state they would be in when the NES resets or boots up.
     * All flags will be set to 0 except 'Unused' and 'InterruptDisable' which will be set to 1.
     */
    void ResetFlags();

    /**
     * Returns the state of the registers as a string
     */
    std::string ToString() const;
};