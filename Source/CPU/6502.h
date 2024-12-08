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
    void LDA(const OpCode& InOpCode);
    void LDX(const OpCode& InOpCode);
    void LDY(const OpCode& InOpCode);

    void STA(const OpCode& InOpCode);
    void STX(const OpCode& InOpCode);
    void STY(const OpCode& InOpCode);

    void TAX(const OpCode& InOpCode);
    void TAY(const OpCode& InOpCode);
    void TXA(const OpCode& InOpCode);
    void TYA(const OpCode& InOpCode);

    void TSX(const OpCode& InOpCode);
    void TXS(const OpCode& InOpCode);
    void PHA(const OpCode& InOpCode);
    void PHP(const OpCode& InOpCode);
    void PLA(const OpCode& InOpCode);
    void PLP(const OpCode& InOpCode);

    void AND(const OpCode& InOpCode);
    void EOR(const OpCode& InOpCode);
    void ORA(const OpCode& InOpCode);
    void BIT(const OpCode& InOpCode);

    void ADC(const OpCode& InOpCode);
    void SBC(const OpCode& InOpCode);
    void CMP(const OpCode& InOpCode);
    void CPX(const OpCode& InOpCode);
    void CPY(const OpCode& InOpCode);

    void INC(const OpCode& InOpCode);
    void INX(const OpCode& InOpCode);
    void INY(const OpCode& InOpCode);
    void DEC(const OpCode& InOpCode);
    void DEX(const OpCode& InOpCode);
    void DEY(const OpCode& InOpCode);

    void ASL(const OpCode& InOpCode);
    void LSR(const OpCode& InOpCode);
    void ROL(const OpCode& InOpCode);
    void ROR(const OpCode& InOpCode);

    void JMP(const OpCode& InOpCode);
    void JSR(const OpCode& InOpCode);
    void RTS(const OpCode& InOpCode);

    void BCC(const OpCode& InOpCode);
    void BCS(const OpCode& InOpCode);
    void BEQ(const OpCode& InOpCode);
    void BMI(const OpCode& InOpCode);
    void BNE(const OpCode& InOpCode);
    void BPL(const OpCode& InOpCode);
    void BVC(const OpCode& InOpCode);
    void BVS(const OpCode& InOpCode);

    void CLC(const OpCode& InOpCode);
    void CLD(const OpCode& InOpCode);
    void CLI(const OpCode& InOpCode);
    void CLV(const OpCode& InOpCode);
    void SEC(const OpCode& InOpCode);
    void SED(const OpCode& InOpCode);
    void SEI(const OpCode& InOpCode);

    void BRK(const OpCode& InOpCode);
    void NOP(const OpCode& InOpCode);
    void RTI(const OpCode& InOpCode);

private:
    Registers m_Registers;

    std::shared_ptr<Memory> m_pMemory;

    std::vector<Instruction> m_InstructionTable;
};