#pragma once

#include "Types/OpCodes.h"
#include "Types/CpuRegisters.h"

class Bus;
struct OpCode;
struct CpuRegisters;

enum class EAddressingMode : uint8_t;

class Cpu
{
    friend class UIManager;

public:
    Cpu(Bus* InDataBus);
    ~Cpu();

    /**
     * Handles a system reset and sets the CPU state accordingly.
     */
    void Reset();

    /**
     * Handles a request to interrupt the CPU.
     * The interrupt can be discarded if the CPU 'InterruptDisable' flag is set.
     */
    void IRQ();

    /**
     * Handles a CPU interrupt request that cannot be masked out by the CPU 'InterruptDisable' flag.
     * NMI interrupts can however be disabled whilst writing to an memory mapped I/O device.
     */
    void NMI();

    /**
     * Executes a single CPU instruction.
     */
    uint8_t Tick();

    /**
     * Returns a copy of the CPU register state
     */
    CpuRegisters GetRegisters() const;

private:
    /**
     * Pushes the specified value to the top of the stack.
     * The stack pointer will be decremented once the operation completes.
     */
    void PushStack(const uint8_t InValue);

    /**
     * Reads the value at the top of the stack.
     * The stack pointer will be incremented before the read operation.
     */
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

    uint8_t INV(const OpCode& InOpCode);

    ///
    /// Undocumented / illegal opcodes. Some ROMs use these so they need to be supported.
    /// Not all of this type of opcodes have been implemented (only the ones tested by nestest).
    /// 
    
    uint8_t DCP(const OpCode& InOpCode);
    uint8_t ISB(const OpCode& InOpCode);
    uint8_t LAX(const OpCode& InOpCode);
    uint8_t RLA(const OpCode& InOpCode);
    uint8_t RRA(const OpCode& InOpCode);
    uint8_t SAX(const OpCode& InOpCode);
    uint8_t SLO(const OpCode& InOpCode);
    uint8_t SRE(const OpCode& InOpCode);

    /**
     * Returns the address to read from the Bus based on the specified addressing mode.
     * Optionally will also notify if the address will cross a page boundry and should incur a performance penalty.
     * 
     * For the 'Relative' addressing mode we will return the final 'Branch To' address, this is worked out by this function as part of 
     * the calcuations to determine if a page boundry was crossed so it makes sense to just return this if we've already figured it out.
     */
    uint16_t GetAddressByAddressingMode(const EAddressingMode InAddressingMode, bool* bOutDidCrossPageBoundry = nullptr) const;

private:
    uint64_t m_CycleCount = 0;

    CpuRegisters m_Registers;

    Bus* m_pDataBus;

    std::vector<Instruction> m_InstructionTable;
};