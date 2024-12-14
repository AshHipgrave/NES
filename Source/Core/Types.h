#pragma once

class Cpu;

/**
 * Represents the status flags of the CPU
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
    * When set the CPU will obey the rules of 'Binary Coded Decimal (BCD)' artuhmetic during addition and subtraction.
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
    * Set if the result of the last operation had bit 7 set to a one.
    */
    Negative = 7,
};

/**
 * Represents different addressing modes supported by the 6502 CPU.
 * Depending on the addressing mode specified will change where the value the CPU instruction operates on is fetched from.
 */
enum class EAddressingMode : uint8_t
{
    /**
    * The memory address is implied within the instruction itself.
    * For example: The 'CLC' instruction doesn't operate on any memory and instead clears the carry flag.
    */
    Implied = 0,

    /**
    * The instruction operates on the Accumulator directly.
    */
    Accumulator = 1,

    /**
    * The instruction operates directly on an 8-bit constant.
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

/**
 * Represents a single opcode supported by the 6502
 */
struct OpCode
{
    /**
    * The size of this opcode. This value is used to increment the program counter when the opcode executes.
    */
    uint8_t Size = 0;

    /**
     * The number of cycles this opcode takes. This will be used to calculate the correct timing for the CPU
     */
    uint8_t CycleCount = 0;

    /**
     * The addressing mode used by this opcode. This will be used to retrieve the correct value from RAM for the opcode to operate on.
     */
    EAddressingMode AddressingMode = EAddressingMode::Implied;
};

/**
 * Represents an instruction that the emulator can execute.
 * An instruction is a combination of opcodes and a function pointer to the function that will handle/execute this opcode.
 */
struct Instruction
{
    /**
     * The opcode for this instruction.
     */
    OpCode Code;

    /**
     * Function pointer to the function that will handle/execute this instruction.
     */
    uint8_t(Cpu::* PFN_OpCodeHandlerFunction)(const OpCode&) = nullptr;
};

/**
 * Represents the 'Registers' contained within a 6502 CPU.
 */
struct Registers
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
     */
    std::bitset<8> Flags = 36; //36 = 00100100 in binary which sets all flags to 0 except the 'Unused' and 'Interrupt Disable' flags which default to 1 when the NES is powered on

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
     * Tests if the specified flag is set or not
     */
    bool IsFlagSet(const ECpuFlag InFlag) const;
};