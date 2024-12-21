#pragma once

class Cpu;
enum class EAddressingMode : uint8_t;

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
    EAddressingMode AddressingMode;
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