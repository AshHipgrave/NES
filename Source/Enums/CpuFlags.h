#pragma once

/**
 * Represents the status flags of the 6502 CPU
 */
enum class ECpuFlag : uint8_t
{
    /**
    * Set if the last operation caused an overflow from bit 7 of the result, or an underflow from bit 0.
    */
    Carry = 0,

    /**
    * Set if the result of the last operation was 0.
    */
    Zero = 1,

    /**
    * When set the CPU will not respond to any interrupts from devices until cleared.
    */
    InterruptDisable = 2,

    /**
    * When set the CPU will obey the rules of 'Binary Coded Decimal (BCD)' arithmetic during addition and subtraction.
    * Note: For the NES this flag is ignored/unused.
    */
    DecimalMode = 3,

    /**
    * Set when the 'BRK' instruction has been executed and an interrupt has been generated to process it.
    */
    Break = 4,

    /**
    * This bit is always unused / ignored on the 6502.
    */
    Unused = 5,

    /**
    * Used during arithmetic operations if the result yeilded an invalid 2's compliment result.
    * For example, adding two positive numbers and ending up with a negative result (e.g. 64 + 64 = -128).
    * This is determined by looking at the carry between bits 6 and 8 and between bit 7 and the carry flag.
    */
    Overflow = 6,

    /**
    * Set if the result of the last operation has bit 7 set to a one.
    * When set this indicates the result was a negative value.
    */
    Negative = 7,
};