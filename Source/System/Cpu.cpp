#include "pch.h"
#include "System/Cpu.h"
#include "Core/Core.h"
#include "Core/Utils.h"
#include "System/Bus.h"
#include "Types/OpCodes.h"
#include "Enums/CpuFlags.h"
#include "Types/CpuRegisters.h"

Cpu::Cpu(Bus* InDataBus)
    : m_pDataBus(InDataBus)
{
    m_Registers = CpuRegisters();

    m_Registers.X = 0;
    m_Registers.Y = 0;
    m_Registers.Accumulator = 0;

    m_Registers.Flags = 36; //36 = 00100100 in binary which sets all flags to 0 except the 'Unused' and 'Interrupt Disable' flags which default to 1 when the NES is powered on

    m_Registers.StackPointer = 0xFD;
    m_Registers.ProgramCounter = 0xFFFC;

    // https://www.masswerk.at/6502/6502_instruction_set.html
    m_InstructionTable =
    {
        { BRK_Implied,       &Cpu::BRK }, { ORA_IndirectX, &Cpu::ORA }, { OP_NotImplemented, &Cpu::INV }, { SLO_IndirectX,     &Cpu::SLO }, { NOP_ZeroPage,  &Cpu::NOP }, { ORA_ZeroPage,  &Cpu::ORA }, { ASL_ZeroPage,  &Cpu::ASL }, { SLO_ZeroPage,      &Cpu::SLO }, { PHP_Implied, &Cpu::PHP }, { ORA_Immediate, &Cpu::ORA }, { ASL_Accumulator, &Cpu::ASL }, { OP_NotImplemented, &Cpu::INV }, { NOP_Absolute,  &Cpu::NOP }, { ORA_Absolute,  &Cpu::ORA }, { ASL_Absolute,      &Cpu::ASL }, { SLO_Absolute,      &Cpu::SLO },
        { BPL_Relative,      &Cpu::BPL }, { ORA_IndirectY, &Cpu::ORA }, { OP_NotImplemented, &Cpu::INV }, { SLO_IndirectY,     &Cpu::SLO }, { NOP_ZeroPageX, &Cpu::NOP }, { ORA_ZeroPageX, &Cpu::ORA }, { ASL_ZeroPageX, &Cpu::ASL }, { SLO_ZeroPageX,     &Cpu::SLO }, { CLC_Implied, &Cpu::CLC }, { ORA_AbsoluteY, &Cpu::ORA }, { NOP_Implied,     &Cpu::NOP }, { SLO_AbsoluteY,     &Cpu::SLO }, { NOP_AbsoluteX, &Cpu::NOP }, { ORA_AbsoluteX, &Cpu::ORA }, { ASL_AbsoluteX,     &Cpu::ASL }, { SLO_AbsoluteX,     &Cpu::SLO },
        { JSR_Absolute,      &Cpu::JSR }, { AND_IndirectX, &Cpu::AND }, { OP_NotImplemented, &Cpu::INV }, { RLA_IndirectX,     &Cpu::RLA }, { BIT_ZeroPage,  &Cpu::BIT }, { AND_ZeroPage,  &Cpu::AND }, { ROL_ZeroPage,  &Cpu::ROL }, { RLA_ZeroPage,      &Cpu::RLA }, { PLP_Implied, &Cpu::PLP }, { AND_Immediate, &Cpu::AND }, { ROL_Accumulator, &Cpu::ROL }, { OP_NotImplemented, &Cpu::INV }, { BIT_Absolute,  &Cpu::BIT }, { AND_Absolute,  &Cpu::AND }, { ROL_Absolute,      &Cpu::ROL }, { RLA_Absolute,      &Cpu::RLA },
        { BMI_Relative,      &Cpu::BMI }, { AND_IndirectY, &Cpu::AND }, { OP_NotImplemented, &Cpu::INV }, { RLA_IndirectY,     &Cpu::RLA }, { NOP_ZeroPageX, &Cpu::NOP }, { AND_ZeroPageX, &Cpu::AND }, { ROL_ZeroPageX, &Cpu::ROL }, { RLA_ZeroPageX,     &Cpu::RLA }, { SEC_Implied, &Cpu::SEC }, { AND_AbsoluteY, &Cpu::AND }, { NOP_Implied,     &Cpu::NOP }, { RLA_AbsoluteY,     &Cpu::RLA }, { NOP_AbsoluteX, &Cpu::NOP }, { AND_AbsoluteX, &Cpu::AND }, { ROL_AbsoluteX,     &Cpu::ROL }, { RLA_AbsoluteX,     &Cpu::RLA },
        { RTI_Implied,       &Cpu::RTI }, { EOR_IndirectX, &Cpu::EOR }, { OP_NotImplemented, &Cpu::INV }, { SRE_IndirectX,     &Cpu::SRE }, { NOP_ZeroPage,  &Cpu::NOP }, { EOR_ZeroPage,  &Cpu::EOR }, { LSR_ZeroPage,  &Cpu::LSR }, { SRE_ZeroPage,      &Cpu::SRE }, { PHA_Implied, &Cpu::PHA }, { EOR_Immediate, &Cpu::EOR }, { LSR_Accumulator, &Cpu::LSR }, { OP_NotImplemented, &Cpu::INV }, { JMP_Absolute,  &Cpu::JMP }, { EOR_Absolute,  &Cpu::EOR }, { LSR_Absolute,      &Cpu::LSR }, { SRE_Absolute,      &Cpu::SRE },
        { BVC_Relative,      &Cpu::BVC }, { EOR_IndirectY, &Cpu::EOR }, { OP_NotImplemented, &Cpu::INV }, { SRE_IndirectY,     &Cpu::SRE }, { NOP_ZeroPageX, &Cpu::NOP }, { EOR_ZeroPageX, &Cpu::EOR }, { LSR_ZeroPageX, &Cpu::LSR }, { SRE_ZeroPageX,     &Cpu::SRE }, { CLI_Implied, &Cpu::CLI }, { EOR_AbsoluteY, &Cpu::EOR }, { NOP_Implied,     &Cpu::NOP }, { SRE_AbsoluteY,     &Cpu::SRE }, { NOP_AbsoluteX, &Cpu::NOP }, { EOR_AbsoluteX, &Cpu::EOR }, { LSR_AbsoluteX,     &Cpu::LSR }, { SRE_AbsoluteX,     &Cpu::SRE },
        { RTS_Implied,       &Cpu::RTS }, { ADC_IndirectX, &Cpu::ADC }, { OP_NotImplemented, &Cpu::INV }, { RRA_IndirectX,     &Cpu::RRA }, { NOP_ZeroPage,  &Cpu::NOP }, { ADC_ZeroPage,  &Cpu::ADC }, { ROR_ZeroPage,  &Cpu::ROR }, { RRA_ZeroPage,      &Cpu::RRA }, { PLA_Implied, &Cpu::PLA }, { ADC_Immediate, &Cpu::ADC }, { ROR_Accumulator, &Cpu::ROR }, { OP_NotImplemented, &Cpu::INV }, { JMP_Indirect,  &Cpu::JMP }, { ADC_Absolute,  &Cpu::ADC }, { ROR_Absolute,      &Cpu::ROR }, { RRA_Absolute,      &Cpu::RRA },
        { BVS_Relative,      &Cpu::BVS }, { ADC_IndirectY, &Cpu::ADC }, { OP_NotImplemented, &Cpu::INV }, { RRA_IndirectY,     &Cpu::RRA }, { NOP_ZeroPageX, &Cpu::NOP }, { ADC_ZeroPageX, &Cpu::ADC }, { ROR_ZeroPageX, &Cpu::ROR }, { RRA_ZeroPageX,     &Cpu::RRA }, { SEI_Implied, &Cpu::SEI }, { ADC_AbsoluteY, &Cpu::ADC }, { NOP_Implied,     &Cpu::NOP }, { RRA_AbsoluteY,     &Cpu::RRA }, { NOP_AbsoluteX, &Cpu::NOP }, { ADC_AbsoluteX, &Cpu::ADC }, { ROR_AbsoluteX,     &Cpu::ROR }, { RRA_AbsoluteX,     &Cpu::RRA },
        { NOP_Immediate,     &Cpu::NOP }, { STA_IndirectX, &Cpu::STA }, { NOP_Immediate,     &Cpu::NOP }, { SAX_IndirectX,     &Cpu::SAX }, { STY_ZeroPage,  &Cpu::STY }, { STA_ZeroPage,  &Cpu::STA }, { STX_ZeroPage,  &Cpu::STX }, { SAX_ZeroPage,      &Cpu::SAX }, { DEY_Implied, &Cpu::DEY }, { NOP_Immediate, &Cpu::NOP }, { TXA_Implied,     &Cpu::TXA }, { OP_NotImplemented, &Cpu::INV }, { STY_Absolute,  &Cpu::STY }, { STA_Absolute,  &Cpu::STA }, { STX_Absolute,      &Cpu::STX }, { SAX_Absolute,      &Cpu::SAX },
        { BCC_Relative,      &Cpu::BCC }, { STA_IndirectY, &Cpu::STA }, { OP_NotImplemented, &Cpu::INV }, { OP_NotImplemented, &Cpu::INV }, { STY_ZeroPageX, &Cpu::STY }, { STA_ZeroPageX, &Cpu::STA }, { STX_ZeroPageY, &Cpu::STX }, { SAX_ZeroPageY,     &Cpu::SAX }, { TYA_Implied, &Cpu::TYA }, { STA_AbsoluteY, &Cpu::STA }, { TXS_Implied,     &Cpu::TXS }, { OP_NotImplemented, &Cpu::INV }, { NOP_AbsoluteX, &Cpu::INV }, { STA_AbsoluteX, &Cpu::STA }, { OP_NotImplemented, &Cpu::INV }, { OP_NotImplemented, &Cpu::INV },
        { LDY_Immediate,     &Cpu::LDY }, { LDA_IndirectX, &Cpu::LDA }, { LDX_Immediate,     &Cpu::LDX }, { LAX_IndirectX,     &Cpu::LAX }, { LDY_ZeroPage,  &Cpu::LDY }, { LDA_ZeroPage,  &Cpu::LDA }, { LDX_ZeroPage,  &Cpu::LDX }, { LAX_ZeroPage,      &Cpu::LAX }, { TAY_Implied, &Cpu::TAY }, { LDA_Immediate, &Cpu::LDA }, { TAX_Implied,     &Cpu::TAX }, { OP_NotImplemented, &Cpu::INV }, { LDY_Absolute,  &Cpu::LDY }, { LDA_Absolute,  &Cpu::LDA }, { LDX_Absolute,      &Cpu::LDX }, { LAX_Absolute,      &Cpu::LAX },
        { BCS_Relative,      &Cpu::BCS }, { LDA_IndirectY, &Cpu::LDA }, { OP_NotImplemented, &Cpu::INV }, { LAX_IndirectY,     &Cpu::LAX }, { LDY_ZeroPageX, &Cpu::LDY }, { LDA_ZeroPageX, &Cpu::LDA }, { LDX_ZeroPageY, &Cpu::LDX }, { LAX_ZeroPageY,     &Cpu::LAX }, { CLV_Implied, &Cpu::CLV }, { LDA_AbsoluteY, &Cpu::LDA }, { TSX_Implied,     &Cpu::TSX }, { OP_NotImplemented, &Cpu::INV }, { LDY_AbsoluteX, &Cpu::LDY }, { LDA_AbsoluteX, &Cpu::LDA }, { LDX_AbsoluteY,     &Cpu::LDX }, { LAX_AbsoluteY,     &Cpu::LAX },
        { CPY_Immediate,     &Cpu::CPY }, { CMP_IndirectX, &Cpu::CMP }, { NOP_Immediate,     &Cpu::NOP }, { DCP_IndirectX,     &Cpu::DCP }, { CPY_ZeroPage,  &Cpu::CPY }, { CMP_ZeroPage,  &Cpu::CMP }, { DEC_ZeroPage,  &Cpu::DEC }, { DCP_ZeroPage,      &Cpu::DCP }, { INY_Implied, &Cpu::INY }, { CMP_Immediate, &Cpu::CMP }, { DEX_Implied,     &Cpu::DEX }, { OP_NotImplemented, &Cpu::INV }, { CPY_Absolute,  &Cpu::CPY }, { CMP_Absolute,  &Cpu::CMP }, { DEC_Absolute,      &Cpu::DEC }, { DCP_Absolute,      &Cpu::DCP },
        { BNE_Relative,      &Cpu::BNE }, { CMP_IndirectY, &Cpu::CMP }, { OP_NotImplemented, &Cpu::INV }, { DCP_IndirectY,     &Cpu::DCP }, { NOP_ZeroPageX, &Cpu::NOP }, { CMP_ZeroPageX, &Cpu::CMP }, { DEC_ZeroPageX, &Cpu::DEC }, { DCP_ZeroPageX,     &Cpu::DCP }, { CLD_Implied, &Cpu::CLD }, { CMP_AbsoluteY, &Cpu::CMP }, { NOP_Implied,     &Cpu::NOP }, { DCP_AbsoluteY,     &Cpu::DCP }, { NOP_AbsoluteX, &Cpu::NOP }, { CMP_AbsoluteX, &Cpu::CMP }, { DEC_AbsoluteX,     &Cpu::DEC }, { DCP_AbsoluteX,     &Cpu::DCP },
        { CPX_Immediate,     &Cpu::CPX }, { SBC_IndirectX, &Cpu::SBC }, { NOP_Immediate,     &Cpu::NOP }, { ISB_IndirectX,     &Cpu::ISB }, { CPX_ZeroPage,  &Cpu::CPX }, { SBC_ZeroPage,  &Cpu::SBC }, { INC_ZeroPage,  &Cpu::INC }, { ISB_ZeroPage,      &Cpu::ISB }, { INX_Implied, &Cpu::INX }, { SBC_Immediate, &Cpu::SBC }, { NOP_Implied,     &Cpu::NOP }, { SBC_Immediate,     &Cpu::SBC }, { CPX_Absolute,  &Cpu::CPX }, { SBC_Absolute,  &Cpu::SBC }, { INC_Absolute,      &Cpu::INC }, { ISB_Absolute,      &Cpu::ISB },
        { BEQ_Relative,      &Cpu::BEQ }, { SBC_IndirectY, &Cpu::SBC }, { OP_NotImplemented, &Cpu::INV }, { ISB_IndirectY,     &Cpu::ISB }, { NOP_ZeroPageX, &Cpu::NOP }, { SBC_ZeroPageX, &Cpu::SBC }, { INC_ZeroPageX, &Cpu::INC }, { ISB_ZeroPageX,     &Cpu::ISB }, { SED_Implied, &Cpu::SED }, { SBC_AbsoluteY, &Cpu::SBC }, { NOP_Implied,     &Cpu::NOP }, { ISB_AbsoluteY,     &Cpu::ISB }, { NOP_AbsoluteX, &Cpu::NOP }, { SBC_AbsoluteX, &Cpu::SBC }, { INC_AbsoluteX,     &Cpu::INC }, { ISB_AbsoluteX,     &Cpu::ISB }
    };
}

Cpu::~Cpu()
{
    m_pDataBus = nullptr;
    m_InstructionTable.clear();
}

void Cpu::Reset()
{
    m_Registers.SetFlag(ECpuFlag::InterruptDisable, true);

// TEMP - Set to '1' to emulate standard NES reset behaviour. Set to '0' to run the nestest ROM in headless mode (we have no PPU so can't run it as a standard NES ROM).
#if 1
    m_Registers.StackPointer -= 3;

    const uint8_t newLow = m_pDataBus->ReadData(0xFFFC);
    const uint8_t newHigh = m_pDataBus->ReadData(0xFFFD);

    m_Registers.ProgramCounter = Utils::MakeDword(newLow, newHigh);
#else
    m_Registers.StackPointer = 0xFD;
    m_Registers.ProgramCounter = 0xC000;
#endif

    m_CycleCount += 7;
}

void Cpu::IRQ()
{
    const uint8_t lowbyte = Utils::GetLowByte(m_Registers.ProgramCounter);
    const uint8_t highbyte = Utils::GetHighByte(m_Registers.ProgramCounter);

    PushStack(highbyte);
    PushStack(lowbyte);
    PushStack(m_Registers.GetFlags());

    const uint8_t newLow = m_pDataBus->ReadData(0xFFEE);
    const uint8_t newHigh = m_pDataBus->ReadData(0xFFEF);

    m_Registers.ProgramCounter = Utils::MakeDword(newLow, newHigh);

    m_CycleCount += 7;
}

void Cpu::NMI()
{
    const uint8_t lowbyte = Utils::GetLowByte(m_Registers.ProgramCounter);
    const uint8_t highbyte = Utils::GetHighByte(m_Registers.ProgramCounter);

    PushStack(highbyte);
    PushStack(lowbyte);
    PushStack(m_Registers.GetFlags());

    const uint8_t newLow = m_pDataBus->ReadData(0xFFFA);
    const uint8_t newHigh = m_pDataBus->ReadData(0xFFFB);

    m_Registers.ProgramCounter = Utils::MakeDword(newLow, newHigh);
}

uint8_t Cpu::Tick()
{
    const CpuRegisters registersBefore = m_Registers;
    const uint64_t cyclesBefore = m_CycleCount;

    const uint8_t opcode = m_pDataBus->ReadData(m_Registers.ProgramCounter);
    const Instruction instruction = m_InstructionTable[opcode];
    
    const uint8_t cycles = (this->*instruction.PFN_OpCodeHandlerFunction)(instruction.Code);
    m_CycleCount += cycles;

    Utils::LogInstruction(instruction.Code, registersBefore, cyclesBefore);

    return cycles;
}

CpuRegisters Cpu::GetRegisters() const
{
    return m_Registers;
}

void Cpu::PushStack(const uint8_t InValue)
{
    m_pDataBus->WriteData(InValue, 0x0100 + m_Registers.StackPointer);
    m_Registers.StackPointer--;
}

uint8_t Cpu::PopStack()
{
    m_Registers.StackPointer++;

    const uint8_t value = m_pDataBus->ReadData(0x0100 + m_Registers.StackPointer);

    return value;
}

uint8_t Cpu::LDA(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    m_Registers.Accumulator = m_pDataBus->ReadData(address);

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::LDX(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    m_Registers.X = m_pDataBus->ReadData(address);

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.X == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.X, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::LDY(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    m_Registers.Y = m_pDataBus->ReadData(address);

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Y == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Y, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::STA(const OpCode& InOpCode)
{
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);

    m_pDataBus->WriteData(m_Registers.Accumulator, address);

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::STX(const OpCode& InOpCode)
{
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);

    m_pDataBus->WriteData(m_Registers.X, address);

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::STY(const OpCode& InOpCode)
{
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);

    m_pDataBus->WriteData(m_Registers.Y, address);

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
    std::bitset<8> flags = m_Registers.GetFlags();
    flags.set(std::size_t(ECpuFlag::Break), true);

    PushStack(static_cast<uint8_t>(flags.to_ulong()));

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
    const bool bIsBreakSet = m_Registers.IsFlagSet(ECpuFlag::Break);

    m_Registers.Flags = PopStack();
    m_Registers.SetFlag(ECpuFlag::Break, bIsBreakSet);
    m_Registers.SetFlag(ECpuFlag::Unused, true);

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::AND(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    m_Registers.Accumulator &= m_pDataBus->ReadData(address);

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::EOR(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    m_Registers.Accumulator ^= m_pDataBus->ReadData(address);

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::ORA(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    m_Registers.Accumulator |= m_pDataBus->ReadData(address);

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::BIT(const OpCode& InOpCode)
{
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);
    const uint8_t value = m_pDataBus->ReadData(address);

    m_Registers.SetFlag(ECpuFlag::Zero, (m_Registers.Accumulator & value) == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, value & 0x80);
    m_Registers.SetFlag(ECpuFlag::Overflow, value & 0x60);

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::ADC(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);
    const uint8_t value = m_pDataBus->ReadData(address);

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
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    const uint8_t value = m_pDataBus->ReadData(address);
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
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    const uint8_t value = m_pDataBus->ReadData(address);
    const uint8_t accumulator = m_Registers.Accumulator;

    const uint8_t result = accumulator - value;

    m_Registers.SetFlag(ECpuFlag::Carry, accumulator >= value);
    m_Registers.SetFlag(ECpuFlag::Zero, result == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(result, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::CPX(const OpCode& InOpCode)
{
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);

    const uint8_t value = m_pDataBus->ReadData(address);
    const uint8_t x = m_Registers.X;

    const uint8_t result = x - value;

    m_Registers.SetFlag(ECpuFlag::Carry, x >= value);
    m_Registers.SetFlag(ECpuFlag::Zero, result == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(result, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::CPY(const OpCode& InOpCode)
{
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);

    const uint8_t value = m_pDataBus->ReadData(address);
    const uint8_t y = m_Registers.Y;

    const uint8_t result = y - value;

    m_Registers.SetFlag(ECpuFlag::Carry, y >= value);
    m_Registers.SetFlag(ECpuFlag::Zero, result == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(result, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::INC(const OpCode& InOpCode)
{
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);
    const uint8_t value = m_pDataBus->ReadData(address);
    const uint8_t result = value + 1;

    m_pDataBus->WriteData(result, address);

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
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);
    const uint8_t value = m_pDataBus->ReadData(address);
    const uint8_t result = value - 1;

    m_pDataBus->WriteData(result, address);

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
    if (InOpCode.AddressingMode == EAddressingMode::Accumulator)
    {
        m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(m_Registers.Accumulator, 7));

        m_Registers.Accumulator = m_Registers.Accumulator << 1;

        m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
        m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));
    }
    else
    {
        const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);
        const uint8_t value = m_pDataBus->ReadData(address);
        const uint8_t result = value << 1;

        m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(value, 7));
        m_Registers.SetFlag(ECpuFlag::Zero, result == 0);
        m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(result, 7));

        m_pDataBus->WriteData(result, address);
    }

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::LSR(const OpCode& InOpCode)
{
    if (InOpCode.AddressingMode == EAddressingMode::Accumulator)
    {
        m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(m_Registers.Accumulator, 0));

        m_Registers.Accumulator = m_Registers.Accumulator >> 1;

        m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    }
    else
    {
        const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);
        const uint8_t value = m_pDataBus->ReadData(address);
        const uint8_t result = value >> 1;

        m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(value, 0));
        m_Registers.SetFlag(ECpuFlag::Zero, result == 0);

        m_pDataBus->WriteData(result, address);
    }

    m_Registers.SetFlag(ECpuFlag::Negative, false);

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::ROL(const OpCode& InOpCode)
{
    if (InOpCode.AddressingMode == EAddressingMode::Accumulator)
    {
        const uint8_t accumulatorBefore = m_Registers.Accumulator;

        m_Registers.Accumulator = m_Registers.Accumulator << 1;

        const bool bCarrySetBefore = m_Registers.IsFlagSet(ECpuFlag::Carry);
        m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(accumulatorBefore, 7));

        if (bCarrySetBefore)
        {
            m_Registers.Accumulator |= 0x01;
        }

        m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
        m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));
    }
    else
    {
        const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);
        const uint8_t value = m_pDataBus->ReadData(address);
        uint8_t result = value << 1;

        const bool bCarrySetBefore = m_Registers.IsFlagSet(ECpuFlag::Carry);
        m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(value, 7));

        if (bCarrySetBefore)
        {
            result |= 0x01;
        }

        m_pDataBus->WriteData(result, address);

        m_Registers.SetFlag(ECpuFlag::Zero, result == 0);
        m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(result, 7));
    }

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::ROR(const OpCode& InOpCode)
{
    if (InOpCode.AddressingMode == EAddressingMode::Accumulator)
    {
        const uint8_t accumulatorBefore = m_Registers.Accumulator;

        m_Registers.Accumulator = m_Registers.Accumulator >> 1;

        const bool bCarrySetBefore = m_Registers.IsFlagSet(ECpuFlag::Carry);
        m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(accumulatorBefore, 0));

        if (bCarrySetBefore)
        {
            m_Registers.Accumulator |= 0x80;
        }

        m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
        m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));
    }
    else
    {
        const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);
        const uint8_t value = m_pDataBus->ReadData(address);
        uint8_t result = value >> 1;

        const bool bCarrySetBefore = m_Registers.IsFlagSet(ECpuFlag::Carry);
        m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(value, 0));

        if (bCarrySetBefore)
        {
            result |= 0x80;
        }

        m_pDataBus->WriteData(result, address);

        m_Registers.SetFlag(ECpuFlag::Zero, result == 0);
        m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(result, 7));
    }

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::JMP(const OpCode& InOpCode)
{
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);
    m_Registers.ProgramCounter = address;

    return InOpCode.CycleCount;
}

uint8_t Cpu::JSR(const OpCode& InOpCode)
{
    const uint16_t returnAddress = (m_Registers.ProgramCounter + InOpCode.Size) - 1;

    const uint8_t pcLow = Utils::GetLowByte(returnAddress);
    const uint8_t pcHigh = Utils::GetHighByte(returnAddress);

    PushStack(pcHigh);
    PushStack(pcLow);

    const uint16_t jumpAddress = GetAddressByAddressingMode(InOpCode.AddressingMode);

    m_Registers.ProgramCounter = jumpAddress;

    return InOpCode.CycleCount;
}

uint8_t Cpu::RTS(const OpCode& InOpCode)
{
    const uint8_t lowbyte = PopStack();
    const uint8_t highbyte = PopStack();

    const uint16_t returnAddress = Utils::MakeDword(lowbyte, highbyte);

    m_Registers.ProgramCounter = returnAddress + 1;

    return InOpCode.CycleCount;
}

uint8_t Cpu::BCC(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t branchAddress = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    if (m_Registers.IsFlagSet(ECpuFlag::Carry) == false)
    {
        m_Registers.ProgramCounter = branchAddress;

        return InOpCode.CycleCount + 1 + (bDidCrossPageBoundry ? 1 : 0);
    }

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::BCS(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t branchAddress = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    if (m_Registers.IsFlagSet(ECpuFlag::Carry) == true)
    {
        m_Registers.ProgramCounter = branchAddress;

        return InOpCode.CycleCount + 1 + (bDidCrossPageBoundry ? 1 : 0);
    }

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::BEQ(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t branchAddress = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    if (m_Registers.IsFlagSet(ECpuFlag::Zero) == true)
    {
        m_Registers.ProgramCounter = branchAddress;

        return InOpCode.CycleCount + 1 + (bDidCrossPageBoundry ? 1 : 0);
    }

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::BMI(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t branchAddress = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    if (m_Registers.IsFlagSet(ECpuFlag::Negative) == true)
    {
        m_Registers.ProgramCounter = branchAddress;

        return InOpCode.CycleCount + 1 + (bDidCrossPageBoundry ? 1 : 0);
    }

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::BNE(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t branchAddress = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    if (m_Registers.IsFlagSet(ECpuFlag::Zero) == false)
    {
        m_Registers.ProgramCounter = branchAddress;

        return InOpCode.CycleCount + 1 + (bDidCrossPageBoundry ? 1 : 0);
    }

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::BPL(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t branchAddress = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    if (m_Registers.IsFlagSet(ECpuFlag::Negative) == false)
    {
        m_Registers.ProgramCounter = branchAddress;

        return InOpCode.CycleCount + 1 + (bDidCrossPageBoundry ? 1 : 0);
    }

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::BVC(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t branchAddress = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    if (m_Registers.IsFlagSet(ECpuFlag::Overflow) == false)
    {
        m_Registers.ProgramCounter = branchAddress;

        return InOpCode.CycleCount + 1 + (bDidCrossPageBoundry ? 1 : 0);
    }

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::BVS(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t branchAddress = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

    if (m_Registers.IsFlagSet(ECpuFlag::Overflow) == true)
    {
        m_Registers.ProgramCounter = branchAddress;

        return InOpCode.CycleCount + 1 + (bDidCrossPageBoundry ? 1 : 0);
    }

    m_Registers.ProgramCounter += InOpCode.Size;

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

    std::bitset<8> flags = m_Registers.GetFlags();
    flags.set(std::size_t(ECpuFlag::Break), true);

    PushStack(static_cast<uint8_t>(flags.to_ulong()));

    const uint8_t newLow = m_pDataBus->ReadData(0xFFFE);
    const uint8_t newHigh = m_pDataBus->ReadData(0xFFFF);

    m_Registers.ProgramCounter = Utils::MakeDword(newLow, newHigh);

    return InOpCode.CycleCount;
}

uint8_t Cpu::NOP(const OpCode& InOpCode)
{
    // Undocumented NOPs can trigger a memory operations across a page boundry which incurs a performance penalty
    if (InOpCode.AddressingMode == EAddressingMode::AbsoluteX)
    {
        bool bDidCrossPageBoundry = false;
        GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);

        m_Registers.ProgramCounter += InOpCode.Size;

        return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
    }
    else
    {
        m_Registers.ProgramCounter += InOpCode.Size;

        return InOpCode.CycleCount;
    }
}

uint8_t Cpu::RTI(const OpCode& InOpCode)
{
    m_Registers.Flags = PopStack();
    m_Registers.SetFlag(ECpuFlag::Unused, true);
    
    const uint8_t lowbyte = PopStack();
    const uint8_t highbyte = PopStack();

    m_Registers.ProgramCounter = Utils::MakeDword(lowbyte, highbyte);

    return InOpCode.CycleCount;
}

uint8_t Cpu::INV(const OpCode& InOpCode)
{
    std::cout << "ERROR: Attempted to execute an illegal opcode." << std::endl;

    EMULATOR_DEBUG_BREAK();

    return InOpCode.CycleCount;
}

uint8_t Cpu::DCP(const OpCode& InOpCode)
{
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);
    uint8_t value = m_pDataBus->ReadData(address);
    value--;

    m_pDataBus->WriteData(value, address);

    const int result = m_Registers.Accumulator - value;

    m_Registers.SetFlag(ECpuFlag::Zero, result == 0);
    m_Registers.SetFlag(ECpuFlag::Carry, m_Registers.Accumulator >= value);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(result, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::ISB(const OpCode& InOpCode)
{
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);
    uint8_t value = m_pDataBus->ReadData(address);
    value++;

    m_pDataBus->WriteData(value, address);

    const int16_t result = m_Registers.Accumulator - value - (m_Registers.IsFlagSet(ECpuFlag::Carry) ? 0 : 1);

    m_Registers.SetFlag(ECpuFlag::Overflow, ((m_Registers.Accumulator ^ value) & (m_Registers.Accumulator ^ result) & 0x80));
    m_Registers.SetFlag(ECpuFlag::Carry, (result & 0xFF00) == 0);

    m_Registers.Accumulator = result & 0xFF;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::LAX(const OpCode& InOpCode)
{
    bool bDidCrossPageBoundry = false;
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode, &bDidCrossPageBoundry);
    const uint8_t value = m_pDataBus->ReadData(address);

    m_Registers.X = value;
    m_Registers.Accumulator = value;

    m_Registers.SetFlag(ECpuFlag::Zero, value == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(value, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return bDidCrossPageBoundry ? InOpCode.CycleCount + 1 : InOpCode.CycleCount;
}

uint8_t Cpu::RLA(const OpCode& InOpCode)
{
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);
    const uint8_t value = m_pDataBus->ReadData(address);

    uint8_t result = value << 1;

    const bool bCarrySetBefore = m_Registers.IsFlagSet(ECpuFlag::Carry);
    m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(value, 7));

    if (bCarrySetBefore)
    {
        result |= 0x01;
    }

    m_pDataBus->WriteData(result, address);

    m_Registers.Accumulator &= result;

    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::RRA(const OpCode& InOpCode)
{
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);
    uint8_t value = m_pDataBus->ReadData(address);

    const bool bCarrySetBefore = m_Registers.IsFlagSet(ECpuFlag::Carry);
    m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(value, 0));

    value >>= 1;

    if (bCarrySetBefore)
    {
        value |= 0x80;
    }

    m_pDataBus->WriteData(value, address);

    uint16_t result = m_Registers.Accumulator + value + (m_Registers.IsFlagSet(ECpuFlag::Carry) ? 1 : 0);

    m_Registers.Accumulator = result & 0xFF;

    m_Registers.SetFlag(ECpuFlag::Carry, result > 255);
    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));
    m_Registers.SetFlag(ECpuFlag::Overflow, ((m_Registers.Accumulator ^ value) & (m_Registers.Accumulator ^ result) & 0x80));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::SAX(const OpCode& InOpCode)
{
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);
    const uint8_t result = m_Registers.Accumulator & m_Registers.X;

    m_pDataBus->WriteData(result, address);

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::SLO(const OpCode& InOpCode)
{
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);
    const uint8_t value = m_pDataBus->ReadData(address);

    const uint8_t result = value << 1;
    
    m_pDataBus->WriteData(result, address);

    m_Registers.Accumulator |= result;

    m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(value, 7));
    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint8_t Cpu::SRE(const OpCode& InOpCode)
{
    const uint16_t address = GetAddressByAddressingMode(InOpCode.AddressingMode);
    const uint8_t value = m_pDataBus->ReadData(address);

    const uint8_t result = value >> 1;

    m_pDataBus->WriteData(result, address);

    m_Registers.Accumulator ^= result;

    m_Registers.SetFlag(ECpuFlag::Carry, Utils::IsBitSet(value, 0));
    m_Registers.SetFlag(ECpuFlag::Zero, m_Registers.Accumulator == 0);
    m_Registers.SetFlag(ECpuFlag::Negative, Utils::IsBitSet(m_Registers.Accumulator, 7));

    m_Registers.ProgramCounter += InOpCode.Size;

    return InOpCode.CycleCount;
}

uint16_t Cpu::GetAddressByAddressingMode(const EAddressingMode InAddressingMode, bool* bOutDidCrossPageBoundry /*= nullptr*/) const
{
    switch (InAddressingMode)
    {
        case EAddressingMode::Absolute:
        {
            const uint8_t lowbyte = m_pDataBus->ReadData(m_Registers.ProgramCounter + 1);
            const uint8_t highbyte = m_pDataBus->ReadData(m_Registers.ProgramCounter + 2);

            const uint16_t address = Utils::MakeDword(lowbyte, highbyte);

            return address;
        }
        case EAddressingMode::AbsoluteX:
        {
            const uint8_t lowbyte =m_pDataBus->ReadData(m_Registers.ProgramCounter + 1);
            const uint8_t highbyte = m_pDataBus->ReadData(m_Registers.ProgramCounter + 2);

            const uint16_t baseAddress = Utils::MakeDword(lowbyte, highbyte);
            const uint16_t finalAddress = baseAddress + m_Registers.X;

            if (bOutDidCrossPageBoundry != nullptr)
            {
                *bOutDidCrossPageBoundry = Utils::DidCrossPageBoundry(baseAddress, finalAddress);
            }

            return finalAddress;
        }
        case EAddressingMode::AbsoluteY:
        {
            const uint8_t lowbyte = m_pDataBus->ReadData(m_Registers.ProgramCounter + 1);
            const uint8_t highbyte = m_pDataBus->ReadData(m_Registers.ProgramCounter + 2);

            const uint16_t baseAddress = Utils::MakeDword(lowbyte, highbyte);
            const uint16_t finalAddress = baseAddress + m_Registers.Y;

            if (bOutDidCrossPageBoundry != nullptr)
            {
                *bOutDidCrossPageBoundry = Utils::DidCrossPageBoundry(baseAddress, finalAddress);
            }

            return finalAddress;
        }
        case EAddressingMode::Immediate:
        {
            return m_Registers.ProgramCounter + 1;
        }
        case EAddressingMode::Indirect:
        {
            const uint8_t pointerLow = m_pDataBus->ReadData(m_Registers.ProgramCounter + 1);
            const uint8_t pointerHigh = m_pDataBus->ReadData(m_Registers.ProgramCounter + 2);

            const uint16_t pointer = Utils::MakeDword(pointerLow, pointerHigh);

            const uint8_t lowbyte = m_pDataBus->ReadData(pointer);

            const uint16_t highbyteAddress = (pointer & 0xFF) == 0xFF ? (pointer & 0xFF00) : (pointer + 1);
            const uint8_t highbyte = m_pDataBus->ReadData(highbyteAddress);

            const uint16_t address = Utils::MakeDword(lowbyte, highbyte);

            return address;
        }
        case EAddressingMode::IndirectX:
        {
            uint8_t effectiveZP = m_pDataBus->ReadData(m_Registers.ProgramCounter + 1);
            effectiveZP = (effectiveZP + m_Registers.X) & 0xFF;

            const uint8_t lowbyte = m_pDataBus->ReadData(effectiveZP);
            const uint8_t highbyte = m_pDataBus->ReadData((effectiveZP + 1) & 0xFF);

            const uint16_t address = Utils::MakeDword(lowbyte, highbyte);

            return address;
        }
        case EAddressingMode::IndirectY:
        {
            const uint8_t zpAddress = m_pDataBus->ReadData(m_Registers.ProgramCounter + 1);

            const uint8_t lowbyte = m_pDataBus->ReadData(zpAddress);
            const uint8_t highbyte = m_pDataBus->ReadData((zpAddress + 1) & 0xFF);

            const uint16_t baseAddress = Utils::MakeDword(lowbyte, highbyte);
            const uint16_t finalAddress = baseAddress + m_Registers.Y;

            if (bOutDidCrossPageBoundry != nullptr)
            {
                *bOutDidCrossPageBoundry = Utils::DidCrossPageBoundry(baseAddress, finalAddress);
            }

            return finalAddress;
        }
        case EAddressingMode::Relative:
        {
            const uint16_t offsetAddress = m_Registers.ProgramCounter + 1;
            const int8_t offset = m_pDataBus->ReadData(offsetAddress);
            
            const uint16_t baseProgramCounter = m_Registers.ProgramCounter + 2;
            const uint16_t branchedProgramCounter = baseProgramCounter + offset;

            if (bOutDidCrossPageBoundry != nullptr)
            {
                *bOutDidCrossPageBoundry = Utils::DidCrossPageBoundry(baseProgramCounter, branchedProgramCounter);
            }

            return branchedProgramCounter;
        }
        case EAddressingMode::ZeroPage:
        {
            return m_pDataBus->ReadData(m_Registers.ProgramCounter + 1);
        }
        case EAddressingMode::ZeroPageX:
        {
            const uint8_t baseAddress = m_pDataBus->ReadData(m_Registers.ProgramCounter + 1);
            const uint16_t address = (baseAddress + m_Registers.X) & 0xFF;

            return address;
        }
        case EAddressingMode::ZeroPageY:
        {
            const uint8_t baseAddress = m_pDataBus->ReadData(m_Registers.ProgramCounter + 1);
            const uint16_t address = (baseAddress + m_Registers.Y) & 0xFF;

            return address;
        }
    }

    std::cout << "Error: Unimplemented address mode" << std::endl;
    EMULATOR_DEBUG_BREAK();

    return 0;
}
