#pragma once

/**
 * Represents different addressing modes supported by the 6502 CPU.
 * Different addressing modes will change where the value the CPU instruction operates on is fetched from.
 */
enum class EAddressingMode : uint8_t
{
    /**
    * The memory address is implied within the instruction itself (or the instruction does not operate on any memory location).
    * For example: The 'CLC' instruction doesn't operate on any memory and instead clears the carry flag.
    */
    Implied = 0,

    /**
    * The instruction operates on the Accumulator directly.
    */
    Accumulator = 1,

    /**
    * The instruction operates directly on an 8-bit constant, which will be the next byte in RAM.
    * For example: In source this would be written as "LDA #10" which would load the value 0x10 into the Accumulator
    */
    Immediate = 2,

    /**
    * The instruction operates on a value within the 'Zero Page' region in RAM (the first 265 bytes).
    * The high bytes of the memory address will therefore always be zero and the low bytes will be populated from the specified constant.
    * For example: In source this would be written as "LDA $FF" which would load the value contained at memory address 0x00FF into the Accumulator.
    */
    ZeroPage = 3,

    /**
    * The instruction operates on a value within the 'Zero Page' region in RAM (the first 265 bytes), indexed via the X register.
    * The high bytes of the memory address will therefore always be zero and the low bytes will be populated from the specified constant and added to the contents of the X-register.
    * For example: In source this would be written as "LDA $80,X" if the X register contained "0x0F" then the final memory address would 0x80 + X = 0x80 + 0x0F = 0x008F
    *
    * Note: The address calculation wraps around if the sum of the base address and the register exceed 0xFF.
    */
    ZeroPageX = 4,

    /**
    * The instruction operates on a value within the 'Zero Page' region in RAM (the first 265 bytes), indexed via the Y register.
    * The high bytes of the memory address will therefore always be zero and the low bytes will be populated from the specified constant and added to the contents of the Y-register.
    * For example: In source this would be written as "LDX $80,Y" if the Y register contained "0x0F" then the final memory address would 0x80 + Y = 0x80 + 0x0F = 0x008F
    *
    * Note: This mode can only be used by the STX and LDX instructions.
    */
    ZeroPageY = 5,

    /**
    * Used by the 'Branch' instruction. The Program Counter will be incremented by an 8-bit signed relative offset if the condition is true.
    */
    Relative = 6,

    /**
    * The memory address read as the 16-bit value in RAM immediately following the opcode.
    * For example: in source this would be written as "JMP $01AA" which would jump to the memory address 0x01AA
    */
    Absolute = 7,

    /**
    * The memory address read as the 16-bit value in RAM immediately following the opcode.
    * The contents of the X register is then added to this to get the final memory address.
    */
    AbsoluteX = 8,

    /**
    * The memory address read as the 16-bit value in RAM immediately following the opcode.
    * The contents of the Y register is then added to this to get the final memory address.
    */
    AbsoluteY = 9,

    /**
    * The memory address read as the 16-bit value in RAM immediately following the opcode.
    * The value at this address is then used as the low byte of the final memory address and the next direct byte in RAM used as the high byte.
    * The opcode then operates on this final memory address.
    */
    Indirect = 10,

    /**
    * The memory address read as the 16-bit value in RAM immediately following the opcode.
    * The value at this address is added to the X register and then this value is used as the low byte of the final memory address (the next direct byte in RAM used as the high byte).
    * The opcode then operates on this final memory address.
    */
    IndirectX = 11,

    /**
    * The memory address read as the 16-bit value in RAM immediately following the opcode.
    * The value at this address is added to the Y register and then this value is used as the low byte of the final memory address (the next direct byte in RAM used as the high byte).
    * The opcode then operates on this final memory address.
    */
    IndirectY = 12
};