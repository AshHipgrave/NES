#include "pch.h"
#include "CPU/6502.h"
#include "Core/Core.h"
#include "Core/Utils.h"
#include "CPU/OpCodes.h"
#include "System/Memory.h"

Cpu::Cpu(std::shared_ptr<Memory> InSystemMemory)
{
    m_pMemory = InSystemMemory;

    m_Registers = Registers();

    m_Registers.X = 0;
    m_Registers.Y = 0;
    m_Registers.Accumulator = 0;

    m_Registers.Flags = 36;

    m_Registers.StackPointer = 0xFD;
    m_Registers.ProgramCounter = 0xFFFC;

    // https://www.masswerk.at/6502/6502_instruction_set.html
    m_InstructionTable =
    {
        { BRK_Implied,       &Cpu::BRK }, { ORA_IndirectX, &Cpu::ORA }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { ORA_ZeroPage,  &Cpu::ORA }, { ASL_ZeroPage,  &Cpu::ASL }, { OP_NotImplemented, &Cpu::NOP }, { PHP_Implied, &Cpu::PHP }, { ORA_Immediate,     &Cpu::ORA }, { ASL_Accumulator,   &Cpu::ASL }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { ORA_Absolute,  &Cpu::ORA }, { ASL_Absolute,      &Cpu::ASL }, { OP_NotImplemented, &Cpu::NOP },
        { BPL_Relative,      &Cpu::BPL }, { ORA_IndirectY, &Cpu::ORA }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { ORA_ZeroPageX, &Cpu::ORA }, { ASL_ZeroPageX, &Cpu::ASL }, { OP_NotImplemented, &Cpu::NOP }, { CLC_Implied, &Cpu::CLC }, { ORA_AbsoluteY,     &Cpu::ORA }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { ORA_AbsoluteX, &Cpu::ORA }, { ASL_AbsoluteX,     &Cpu::ASL }, { OP_NotImplemented, &Cpu::NOP },
        { JSR_Absolute,      &Cpu::JSR }, { AND_IndirectX, &Cpu::AND }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { BIT_ZeroPage,      &Cpu::BIT }, { AND_ZeroPage,  &Cpu::AND }, { ROL_ZeroPage,  &Cpu::ROL }, { OP_NotImplemented, &Cpu::NOP }, { PLP_Implied, &Cpu::PLP }, { AND_Immediate,     &Cpu::AND }, { ROL_Accumulator,   &Cpu::ROL }, { OP_NotImplemented, &Cpu::NOP }, { BIT_Absolute,      &Cpu::BIT }, { AND_Absolute,  &Cpu::AND }, { ROL_Absolute,      &Cpu::ROL }, { OP_NotImplemented, &Cpu::NOP },
        { BMI_Relative,      &Cpu::BMI }, { AND_IndirectY, &Cpu::AND }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { AND_ZeroPageX, &Cpu::AND }, { ROL_ZeroPageX, &Cpu::ROL }, { OP_NotImplemented, &Cpu::NOP }, { SEC_Implied, &Cpu::SEC }, { AND_AbsoluteY,     &Cpu::AND }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { AND_AbsoluteX, &Cpu::AND }, { ROL_AbsoluteX,     &Cpu::ROL }, { OP_NotImplemented, &Cpu::NOP },
        { RTI_Implied,       &Cpu::RTI }, { EOR_IndirectX, &Cpu::EOR }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { EOR_ZeroPage,  &Cpu::EOR }, { LSR_ZeroPage,  &Cpu::LSR }, { OP_NotImplemented, &Cpu::NOP }, { PHA_Implied, &Cpu::PHA }, { EOR_Immediate,     &Cpu::EOR }, { LSR_Accumulator,   &Cpu::LSR }, { OP_NotImplemented, &Cpu::NOP }, { JMP_Absolute,      &Cpu::JMP }, { EOR_Absolute,  &Cpu::EOR }, { LSR_Absolute,      &Cpu::LSR }, { OP_NotImplemented, &Cpu::NOP },
        { BVC_Relative,      &Cpu::BVC }, { EOR_IndirectY, &Cpu::EOR }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { EOR_ZeroPageX, &Cpu::EOR }, { LSR_ZeroPageX, &Cpu::LSR }, { OP_NotImplemented, &Cpu::NOP }, { CLI_Implied, &Cpu::CLI }, { EOR_AbsoluteY,     &Cpu::EOR }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { EOR_AbsoluteX, &Cpu::EOR }, { LSR_AbsoluteX,     &Cpu::LSR }, { OP_NotImplemented, &Cpu::NOP },
        { RTS_Implied,       &Cpu::RTS }, { ADC_IndirectX, &Cpu::ADC }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { ADC_ZeroPage,  &Cpu::ADC }, { ROR_ZeroPage,  &Cpu::ROR }, { OP_NotImplemented, &Cpu::NOP }, { PLA_Implied, &Cpu::PLA }, { ADC_Immediate,     &Cpu::ADC }, { ROR_Accumulator,   &Cpu::ROR }, { OP_NotImplemented, &Cpu::NOP }, { JMP_Indirect,      &Cpu::JMP }, { ADC_Absolute,  &Cpu::ADC }, { ROR_Absolute,      &Cpu::ROR }, { OP_NotImplemented, &Cpu::NOP },
        { BVS_Relative,      &Cpu::BVS }, { ADC_IndirectY, &Cpu::ADC }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { ADC_ZeroPageX, &Cpu::ADC }, { ROR_ZeroPageX, &Cpu::ROR }, { OP_NotImplemented, &Cpu::NOP }, { SEI_Implied, &Cpu::SEI }, { ADC_AbsoluteY,     &Cpu::ADC }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { ADC_AbsoluteX, &Cpu::ADC }, { ROR_AbsoluteX,     &Cpu::ROR }, { OP_NotImplemented, &Cpu::NOP },
        { OP_NotImplemented, &Cpu::NOP }, { STA_IndirectX, &Cpu::STA }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { STY_ZeroPage,      &Cpu::STY }, { STA_ZeroPage,  &Cpu::STA }, { STX_ZeroPage,  &Cpu::STX }, { OP_NotImplemented, &Cpu::NOP }, { DEY_Implied, &Cpu::DEY }, { OP_NotImplemented, &Cpu::NOP }, { TXA_Implied,       &Cpu::TXA }, { OP_NotImplemented, &Cpu::NOP }, { STY_Absolute,      &Cpu::STY }, { STA_Absolute,  &Cpu::STA }, { STX_Absolute,      &Cpu::STX }, { OP_NotImplemented, &Cpu::NOP },
        { BCC_Relative,      &Cpu::BCC }, { STA_IndirectY, &Cpu::STA }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { STY_ZeroPageX,     &Cpu::STY }, { STA_ZeroPageX, &Cpu::STA }, { STX_ZeroPageY, &Cpu::STX }, { OP_NotImplemented, &Cpu::NOP }, { TYA_Implied, &Cpu::TYA }, { STA_AbsoluteY,     &Cpu::STA }, { TXS_Implied,       &Cpu::TXS }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { STA_AbsoluteX, &Cpu::STA }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP },
        { LDY_Immediate,     &Cpu::LDY }, { LDA_IndirectX, &Cpu::LDA }, { LDX_Immediate,     &Cpu::LDX }, { OP_NotImplemented, &Cpu::NOP }, { LDY_ZeroPage,      &Cpu::LDY }, { LDA_ZeroPage,  &Cpu::LDA }, { LDX_ZeroPage,  &Cpu::LDX }, { OP_NotImplemented, &Cpu::NOP }, { TAY_Implied, &Cpu::TAY }, { LDA_Immediate,     &Cpu::LDA }, { TAX_Implied,       &Cpu::TAX }, { OP_NotImplemented, &Cpu::NOP }, { LDY_Absolute,      &Cpu::LDY }, { LDA_Absolute,  &Cpu::LDA }, { LDX_Absolute,      &Cpu::LDX }, { OP_NotImplemented, &Cpu::NOP },
        { BCS_Relative,      &Cpu::BCS }, { LDA_IndirectY, &Cpu::LDA }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { LDY_ZeroPageX,     &Cpu::LDY }, { LDA_ZeroPageX, &Cpu::LDA }, { LDX_ZeroPageY, &Cpu::LDX }, { OP_NotImplemented, &Cpu::NOP }, { CLV_Implied, &Cpu::CLV }, { LDA_AbsoluteY,     &Cpu::LDA }, { TSX_Implied,       &Cpu::TSX }, { OP_NotImplemented, &Cpu::NOP }, { LDY_AbsoluteX,     &Cpu::LDX }, { LDA_AbsoluteX, &Cpu::LDA }, { LDX_AbsoluteY,     &Cpu::LDX }, { OP_NotImplemented, &Cpu::NOP },
        { CPY_Immediate,     &Cpu::CPY }, { CMP_IndirectX, &Cpu::CMP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { CPY_ZeroPage,      &Cpu::CPY }, { CMP_ZeroPage,  &Cpu::CMP }, { DEC_ZeroPage,  &Cpu::DEC }, { OP_NotImplemented, &Cpu::NOP }, { INY_Implied, &Cpu::INY }, { CMP_Immediate,     &Cpu::CMP }, { DEX_Implied,       &Cpu::DEX }, { OP_NotImplemented, &Cpu::NOP }, { CPY_Absolute,      &Cpu::CPY }, { CMP_Absolute,  &Cpu::CMP }, { DEC_Absolute,      &Cpu::DEC }, { OP_NotImplemented, &Cpu::NOP },
        { BNE_Relative,      &Cpu::BNE }, { CMP_IndirectY, &Cpu::CMP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { CMP_ZeroPageX, &Cpu::CMP }, { DEC_ZeroPageX, &Cpu::DEC }, { OP_NotImplemented, &Cpu::NOP }, { CLD_Implied, &Cpu::CLD }, { CMP_AbsoluteY,     &Cpu::CMP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { CMP_AbsoluteX, &Cpu::CMP }, { DEC_AbsoluteX,     &Cpu::DEC }, { OP_NotImplemented, &Cpu::NOP },
        { CPX_Immediate,     &Cpu::CPX }, { SBC_IndirectX, &Cpu::SBC }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { CPX_ZeroPage,      &Cpu::CPX }, { SBC_ZeroPage,  &Cpu::SBC }, { INC_ZeroPage,  &Cpu::INC }, { OP_NotImplemented, &Cpu::NOP }, { INX_Implied, &Cpu::INX }, { SBC_Immediate,     &Cpu::SBC }, { NOP_Implied,       &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { CPX_Absolute,      &Cpu::CPX }, { SBC_Absolute,  &Cpu::SBC }, { INC_Absolute,      &Cpu::INC }, { OP_NotImplemented, &Cpu::NOP },
        { BEQ_Relative,      &Cpu::BEQ }, { SBC_IndirectY, &Cpu::SBC }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { SBC_ZeroPageX, &Cpu::SBC }, { INC_ZeroPageX, &Cpu::INC }, { OP_NotImplemented, &Cpu::NOP }, { SED_Implied, &Cpu::SED }, { SBC_AbsoluteY,     &Cpu::SBC }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { OP_NotImplemented, &Cpu::NOP }, { SBC_AbsoluteX, &Cpu::SBC }, { INC_AbsoluteX,     &Cpu::INC }, { OP_NotImplemented, &Cpu::NOP }
    };
}

Cpu::~Cpu()
{
    m_InstructionTable.clear();
}

void Cpu::Reset()
{
    m_Registers.SetFlag(ECpuFlag::InterruptDisable, true);

    m_Registers.StackPointer -= 3;
    m_Registers.ProgramCounter = 0xFFFC;
}

void Cpu::IRQ()
{

}

void Cpu::NMI()
{

}

void Cpu::Tick()
{
    const uint8_t opcode = m_pMemory->ReadByte(m_Registers.ProgramCounter);
    const Instruction instruction = m_InstructionTable[opcode];
    
    const uint8_t cycles = (this->*instruction.PFN_OpCodeHandlerFunction)(instruction.Code);

    // TODO: Delay for how long the previous instruction took (e.g. if it took 3 cycles, delay for the equivelent of 3 cycles).
    UNUSED_PARAMETER(cycles);
}

Registers Cpu::GetCpuState() const
{
    return m_Registers;
}

void Cpu::PushStack(const uint8_t InValue)
{
    m_pMemory->WriteByte(InValue, 0x0100 + m_Registers.StackPointer);
    m_Registers.StackPointer--;
}

uint8_t Cpu::PopStack()
{
    const uint8_t value = m_pMemory->ReadByte(0x0100 + m_Registers.StackPointer);
    m_Registers.StackPointer++;

    return value;
}

uint8_t Cpu::LDA(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint8_t value = m_pMemory->ReadViaAddressingMode(m_Registers, InOpCode.AddressingMode, &bDidCrossPageBoundry);

    m_Registers.Accumulator = value;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::LDX(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint8_t value = m_pMemory->ReadViaAddressingMode(m_Registers, InOpCode.AddressingMode, &bDidCrossPageBoundry);

    m_Registers.X = value;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.X == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.X, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::LDY(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint8_t value = m_pMemory->ReadViaAddressingMode(m_Registers, InOpCode.AddressingMode, &bDidCrossPageBoundry);

    m_Registers.Y = value;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Y == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Y, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::STA(const OpCode& InOpCode)
{
    const uint16_t address = m_pMemory->ReadViaAddressingMode(m_Registers, InOpCode.AddressingMode);

    m_pMemory->WriteByte(m_Registers.Accumulator, address);

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::STX(const OpCode& InOpCode)
{
    const uint16_t address = m_pMemory->ReadViaAddressingMode(m_Registers, InOpCode.AddressingMode);

    m_pMemory->WriteByte(m_Registers.X, address);

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::STY(const OpCode& InOpCode)
{
    const uint16_t address = m_pMemory->ReadViaAddressingMode(m_Registers, InOpCode.AddressingMode);

    m_pMemory->WriteByte(m_Registers.Y, address);

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::TAX(const OpCode& InOpCode)
{
    m_Registers.X = m_Registers.Accumulator;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.X == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.X, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::TAY(const OpCode& InOpCode)
{
    m_Registers.Y = m_Registers.Accumulator;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Y == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Y, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::TXA(const OpCode& InOpCode)
{
    m_Registers.Accumulator = m_Registers.X;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::TYA(const OpCode& InOpCode)
{
    m_Registers.Accumulator = m_Registers.Y;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::TSX(const OpCode& InOpCode)
{
    m_Registers.X = m_Registers.StackPointer;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.X == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.X, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::TXS(const OpCode& InOpCode)
{
    m_Registers.StackPointer = m_Registers.X;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.X == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.X, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::PHA(const OpCode& InOpCode)
{
    PushStack(m_Registers.Accumulator);

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::PHP(const OpCode& InOpCode)
{
    PushStack(m_Registers.GetFlags());

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::PLA(const OpCode& InOpCode)
{
    m_Registers.Accumulator = PopStack();

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::PLP(const OpCode& InOpCode)
{
    m_Registers.Flags = PopStack();

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::AND(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint8_t value = m_pMemory->ReadViaAddressingMode(m_Registers, InOpCode.AddressingMode, &bDidCrossPageBoundry);

    m_Registers.Accumulator &= value;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::EOR(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint8_t value = m_pMemory->ReadViaAddressingMode(m_Registers, InOpCode.AddressingMode, &bDidCrossPageBoundry);

    m_Registers.Accumulator ^= value;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::ORA(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint8_t value = m_pMemory->ReadViaAddressingMode(m_Registers, InOpCode.AddressingMode, &bDidCrossPageBoundry);

    m_Registers.Accumulator |= value;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::BIT(const OpCode& InOpCode)
{
    const uint8_t value = m_pMemory->ReadViaAddressingMode(m_Registers, InOpCode.AddressingMode);

    m_Registers.SetFlag(ECpuFlag::Zero, (m_Registers.Accumulator & value) == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, value & 0x80);
    m_Registers.SetFlag(ECpuFlag::Overflow, value & 0x60);

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::ADC(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint8_t value = m_pMemory->ReadViaAddressingMode(m_Registers, InOpCode.AddressingMode, &bDidCrossPageBoundry);

    const uint8_t accumulator = m_Registers.Accumulator;
    const uint8_t carry = m_Registers.IsFlagSet(ECpuFlag::Carry) ? 1 : 0;

    const uint16_t tempResult = accumulator + value + carry;
    const uint8_t result = accumulator + value + carry;
    
    m_Registers.SetFlag(ECpuFlag::Carry, tempResult > 255);
    m_Registers.SetFlag(ECpuFlag::Zero, result == 0);
    m_Registers.SetFlag(ECpuFlag::Overflow, (result ^ accumulator) & (result ^ value) & 0x80);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(result, 7));

    m_Registers.Accumulator = result;

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::SBC(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint8_t value = m_pMemory->ReadViaAddressingMode(m_Registers, InOpCode.AddressingMode, &bDidCrossPageBoundry);
    const uint8_t invertedValue = ~value;

    const uint8_t accumulator = m_Registers.Accumulator;
    const uint8_t carry = m_Registers.IsFlagSet(ECpuFlag::Carry) ? 1 : 0;

    const uint16_t tempResult = accumulator + invertedValue + carry;
    const uint8_t result = accumulator + invertedValue + carry;

    m_Registers.SetFlag(ECpuFlag::Carry, tempResult > 255);
    m_Registers.SetFlag(ECpuFlag::Zero, result == 0);
    m_Registers.SetFlag(ECpuFlag::Overflow, (result ^ accumulator) & (result ^ invertedValue) & 0x80);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(result, 7));

    m_Registers.Accumulator = result;

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::CMP(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint8_t value = m_pMemory->ReadViaAddressingMode(m_Registers, InOpCode.AddressingMode, &bDidCrossPageBoundry);

    const uint8_t accumulator = m_Registers.Accumulator;

    m_Registers.SetFlag(ECpuFlag::Carry, accumulator >= value);
    m_Registers.SetFlag(ECpuFlag::Zero, accumulator == value);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(value, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::CPX(const OpCode& InOpCode)
{
    const uint8_t value = m_pMemory->ReadViaAddressingMode(m_Registers, InOpCode.AddressingMode);

    const uint8_t X = m_Registers.X;

    m_Registers.SetFlag(ECpuFlag::Carry, X >= value);
    m_Registers.SetFlag(ECpuFlag::Zero, X == value);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(value, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::CPY(const OpCode& InOpCode)
{
    const uint8_t value = m_pMemory->ReadViaAddressingMode(m_Registers, InOpCode.AddressingMode);

    const uint8_t Y = m_Registers.Y;

    m_Registers.SetFlag(ECpuFlag::Carry, Y >= value);
    m_Registers.SetFlag(ECpuFlag::Zero, Y == value);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(value, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::INC(const OpCode& InOpCode)
{
    const uint16_t address = m_pMemory->GetAddress(m_Registers, InOpCode.AddressingMode);
    const uint8_t value = m_pMemory->ReadByte(address);
    const uint8_t result = value + 1;

    m_pMemory->WriteByte(result, address);

    m_Registers.SetFlag(ECpuFlag::Zero, result == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(result, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::INX(const OpCode& InOpCode)
{
    m_Registers.X++;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.X == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.X, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::INY(const OpCode& InOpCode)
{
    m_Registers.Y++;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Y == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Y, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::DEC(const OpCode& InOpCode)
{
    const uint16_t address = m_pMemory->GetAddress(m_Registers, InOpCode.AddressingMode);
    const uint8_t value = m_pMemory->ReadByte(address);
    const uint8_t result = value - 1;

    m_pMemory->WriteByte(result, address);

    m_Registers.SetFlag(ECpuFlag::Zero, result == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(result, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::DEX(const OpCode& InOpCode)
{
    m_Registers.X--;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.X == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.X, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::DEY(const OpCode& InOpCode)
{
    m_Registers.Y--;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Y == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Y, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::ASL(const OpCode& InOpCode)
{
    const uint16_t address = m_pMemory->GetAddress(m_Registers, InOpCode.AddressingMode);
    const uint8_t value = m_pMemory->ReadByte(address);
    const uint8_t result = value << 1;

    m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(value, 7));
    m_Registers.SetFlag(ECpuFlag::Zero, result == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(result, 7));

    m_pMemory->WriteByte(result, address);

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::LSR(const OpCode& InOpCode)
{
    const uint16_t address = m_pMemory->GetAddress(m_Registers, InOpCode.AddressingMode);
    const uint8_t value = m_pMemory->ReadByte(address);
    const uint8_t result = value >> 1;

    m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(value, 7));
    m_Registers.SetFlag(ECpuFlag::Zero, result == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(result, 7));

    m_pMemory->WriteByte(result, address);

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::ROL(const OpCode& InOpCode)
{
    const uint16_t address = m_pMemory->GetAddress(m_Registers, InOpCode.AddressingMode);
    const uint8_t value = m_pMemory->ReadByte(address);
    uint8_t result = value << 1;

    const bool bCarrySetBefore = m_Registers.IsFlagSet(ECpuFlag::Carry);
    m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(value, 7));

    if (bCarrySetBefore)
    {
        result |= 0x01;
    }

    m_pMemory->WriteByte(result, address);

    m_Registers.SetFlag(ECpuFlag::Zero, result == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(result, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::ROR(const OpCode& InOpCode)
{
    const uint16_t address = m_pMemory->GetAddress(m_Registers, InOpCode.AddressingMode);
    const uint8_t value = m_pMemory->ReadByte(address);
    uint8_t result = value >> 1;

    const bool bCarrySetBefore = m_Registers.IsFlagSet(ECpuFlag::Carry);
    m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(value, 1));

    if (bCarrySetBefore)
    {
        result |= 0x08;
    }

    m_pMemory->WriteByte(result, address);

    m_Registers.SetFlag(ECpuFlag::Zero, result == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(result, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::JMP(const OpCode& InOpCode)
{
    const uint16_t address = m_pMemory->GetAddress(m_Registers, InOpCode.AddressingMode);

    const uint8_t lowbyte = m_pMemory->ReadByte(address);
    const uint8_t highbyte = m_pMemory->ReadByte(address + 1);

    m_Registers.ProgramCounter = Utils::Combine(lowbyte, highbyte);

    return InOpCode.CycleCount;
}

uint8_t Cpu::JSR(const OpCode& InOpCode)
{
    const uint8_t pcLow = Utils::GetLowByte(m_Registers.ProgramCounter);
    const uint8_t pcHigh = Utils::GetHighByte(m_Registers.ProgramCounter);

    PushStack(pcHigh);
    PushStack(pcLow);

    const uint16_t address = m_pMemory->GetAddress(m_Registers, InOpCode.AddressingMode);

    const uint8_t lowbyte = m_pMemory->ReadByte(address);
    const uint8_t highbyte = m_pMemory->ReadByte(address + 1);

    m_Registers.ProgramCounter = Utils::Combine(lowbyte, highbyte);

    return InOpCode.CycleCount;
}

uint8_t Cpu::RTS(const OpCode& InOpCode)
{
    const uint8_t lowbyte = PopStack();
    const uint8_t highbyte = PopStack();

    m_Registers.ProgramCounter = Utils::Combine(lowbyte, highbyte);
    m_Registers.ProgramCounter++;

    return InOpCode.CycleCount;
}

uint8_t Cpu::BCC(const OpCode& InOpCode)
{
    UNUSED_PARAMETER(InOpCode);

    return InOpCode.CycleCount;
}

uint8_t Cpu::BCS(const OpCode& InOpCode)
{
    UNUSED_PARAMETER(InOpCode);

    return InOpCode.CycleCount;
}

uint8_t Cpu::BEQ(const OpCode& InOpCode)
{
    UNUSED_PARAMETER(InOpCode);

    return InOpCode.CycleCount;
}

uint8_t Cpu::BMI(const OpCode& InOpCode)
{
    UNUSED_PARAMETER(InOpCode);

    return InOpCode.CycleCount;
}

uint8_t Cpu::BNE(const OpCode& InOpCode)
{
    UNUSED_PARAMETER(InOpCode);

    return InOpCode.CycleCount;
}

uint8_t Cpu::BPL(const OpCode& InOpCode)
{
    UNUSED_PARAMETER(InOpCode);

    return InOpCode.CycleCount;
}

uint8_t Cpu::BVC(const OpCode& InOpCode)
{
    UNUSED_PARAMETER(InOpCode);

    return InOpCode.CycleCount;
}

uint8_t Cpu::BVS(const OpCode& InOpCode)
{
    UNUSED_PARAMETER(InOpCode);

    return InOpCode.CycleCount;
}

uint8_t Cpu::CLC(const OpCode& InOpCode)
{
    m_Registers.SetFlag(ECpuFlag::Carry, false);

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::CLD(const OpCode& InOpCode)
{
    m_Registers.SetFlag(ECpuFlag::DecimalMode, false);
    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::CLI(const OpCode& InOpCode)
{
    m_Registers.SetFlag(ECpuFlag::InterruptDisable, false);
    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::CLV(const OpCode& InOpCode)
{
    m_Registers.SetFlag(ECpuFlag::Overflow, false);
    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::SEC(const OpCode& InOpCode)
{
    m_Registers.SetFlag(ECpuFlag::Carry, true);
    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::SED(const OpCode& InOpCode)
{
    m_Registers.SetFlag(ECpuFlag::DecimalMode, true);
    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::SEI(const OpCode& InOpCode)
{
    m_Registers.SetFlag(ECpuFlag::InterruptDisable, true);
    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::BRK(const OpCode& InOpCode)
{
    m_Registers.SetFlag(ECpuFlag::Break, true);

    const uint8_t lowbyte = Utils::GetLowByte(m_Registers.ProgramCounter);
    const uint8_t highbyte = Utils::GetHighByte(m_Registers.ProgramCounter);

    PushStack(highbyte);
    PushStack(lowbyte);
    PushStack(m_Registers.GetFlags());

    m_Registers.ProgramCounter = 0xFFFE;

    return InOpCode.CycleCount;
}

uint8_t Cpu::NOP(const OpCode& InOpCode)
{
    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::RTI(const OpCode& InOpCode)
{
    m_Registers.Flags = PopStack();
    
    const uint8_t lowbyte = PopStack();
    const uint8_t highbyte = PopStack();

    m_Registers.ProgramCounter = Utils::Combine(lowbyte, highbyte);;

    return InOpCode.CycleCount;
}
