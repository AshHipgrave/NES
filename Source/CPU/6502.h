#pragma once

#include "Cpu/OpCodes.h"

class Memory;

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
    std::bitset<8> Flags = 36; //36 = 00100100 in binary which sets all flags to 0 except the 'Unused' and 'Interrupt Disable' flags which are default-initialised to 1

    uint8_t GetFlags() const
    {
        return static_cast<uint8_t>(Flags.to_ulong());
    }

    void SetFlag(const ECpuFlag InFlag, const bool bInShouldSet)
    {
        Flags.set(std::size_t(InFlag), bInShouldSet);
    }

    bool IsFlagSet(const ECpuFlag InFlag)
    {
        return Flags.test(std::size_t(InFlag));
    }
};

class Cpu
{
public:
    Cpu(std::shared_ptr<Memory> InSystemMemory);
    ~Cpu();

    void Reset();
    void IRQ();
    void NMI();

    void Tick();

    Registers GetCpuState() const;

private:
    void PushStack(const uint8_t InValue);
    uint8_t PopStack();

    uint8_t LDA(const OpCode& InOpCode);
    uint8_t LDX(const OpCode& InOpCode);
    uint8_t LDY(const OpCode& InOpCode);

    uint8_t STA(const OpCode& InOpCode);
    uint8_t STX(const OpCode& InOpCode);
    uint8_t STY(const OpCode& InOpCode);

    uint8_t TAX(const OpCode& InOpCode);
    uint8_t TAY(const OpCode& InOpCode);
    uint8_t TXA(const OpCode& InOpCode);
    uint8_t TYA(const OpCode& InOpCode);

    uint8_t TSX(const OpCode& InOpCode);
    uint8_t TXS(const OpCode& InOpCode);
    uint8_t PHA(const OpCode& InOpCode);
    uint8_t PHP(const OpCode& InOpCode);
    uint8_t PLA(const OpCode& InOpCode);
    uint8_t PLP(const OpCode& InOpCode);

    uint8_t AND(const OpCode& InOpCode);
    uint8_t EOR(const OpCode& InOpCode);
    uint8_t ORA(const OpCode& InOpCode);
    uint8_t BIT(const OpCode& InOpCode);

    uint8_t ADC(const OpCode& InOpCode);
    uint8_t SBC(const OpCode& InOpCode);
    uint8_t CMP(const OpCode& InOpCode);
    uint8_t CPX(const OpCode& InOpCode);
    uint8_t CPY(const OpCode& InOpCode);

    uint8_t INC(const OpCode& InOpCode);
    uint8_t INX(const OpCode& InOpCode);
    uint8_t INY(const OpCode& InOpCode);
    uint8_t DEC(const OpCode& InOpCode);
    uint8_t DEX(const OpCode& InOpCode);
    uint8_t DEY(const OpCode& InOpCode);

    uint8_t ASL(const OpCode& InOpCode);
    uint8_t LSR(const OpCode& InOpCode);
    uint8_t ROL(const OpCode& InOpCode);
    uint8_t ROR(const OpCode& InOpCode);

    uint8_t JMP(const OpCode& InOpCode);
    uint8_t JSR(const OpCode& InOpCode);
    uint8_t RTS(const OpCode& InOpCode);

    uint8_t BCC(const OpCode& InOpCode);
    uint8_t BCS(const OpCode& InOpCode);
    uint8_t BEQ(const OpCode& InOpCode);
    uint8_t BMI(const OpCode& InOpCode);
    uint8_t BNE(const OpCode& InOpCode);
    uint8_t BPL(const OpCode& InOpCode);
    uint8_t BVC(const OpCode& InOpCode);
    uint8_t BVS(const OpCode& InOpCode);

    uint8_t CLC(const OpCode& InOpCode);
    uint8_t CLD(const OpCode& InOpCode);
    uint8_t CLI(const OpCode& InOpCode);
    uint8_t CLV(const OpCode& InOpCode);
    uint8_t SEC(const OpCode& InOpCode);
    uint8_t SED(const OpCode& InOpCode);
    uint8_t SEI(const OpCode& InOpCode);

    uint8_t BRK(const OpCode& InOpCode);
    uint8_t NOP(const OpCode& InOpCode);
    uint8_t RTI(const OpCode& InOpCode);

private:
    Registers m_Registers;

    std::shared_ptr<Memory> m_pMemory;

    std::vector<Instruction> m_InstructionTable;
};