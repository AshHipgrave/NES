#include "pch.h"
#include "Core/Utils.h"
#include "Core/Types.h"
#include "System/Bus.h"

const std::vector<std::string> OpCodeStrings = 
{
     "BRK", "ORA", "???", "???", "???", "ORA", "ASL", "???", "PHP", "ORA", "ASL", "???", "???", "ORA", "ASL", "???",
     "BPL", "ORA", "???", "???", "???", "ORA", "ASL", "???", "CLC", "ORA", "???", "???", "???", "ORA", "ASL", "???",
     "JSR", "AND", "???", "???", "BIT", "AND", "ROL", "???", "PLP", "AND", "ROL", "???", "BIT", "AND", "ROL", "???",
     "BMI", "AND", "???", "???", "???", "AND", "ROL", "???", "SEC", "AND", "???", "???", "???", "AND", "ROL", "???",
     "RTI", "EOR", "???", "???", "???", "EOR", "LSR", "???", "PHA", "EOR", "LSR", "???", "JMP", "EOR", "LSR", "???",
     "BVC", "EOR", "???", "???", "???", "EOR", "LSR", "???", "CLI", "EOR", "???", "???", "???", "EOR", "LSR", "???",
     "RTS", "ADC", "???", "???", "???", "ADC", "ROR", "???", "PLA", "ADC", "ROR", "???", "JMP", "ADC", "ROR", "???",
     "BVS", "ADC", "???", "???", "???", "ADC", "ROR", "???", "SEI", "ADC", "???", "???", "???", "ADC", "ROR", "???",
     "???", "STA", "???", "???", "STY", "STA", "STX", "???", "DEY", "???", "TXA", "???", "STY", "STA", "STX", "???",
     "BCC", "STA", "???", "???", "STY", "STA", "STX", "???", "TYA", "STA", "TXS", "???", "???", "STA", "???", "???",
     "LDY", "LDA", "LDX", "???", "LDY", "LDA", "LDX", "???", "TAY", "LDA", "TAX", "???", "LDY", "LDA", "LDX", "???",
     "BCS", "LDA", "???", "???", "LDY", "LDA", "LDX", "???", "CLV", "LDA", "TSX", "???", "LDX", "LDA", "LDX", "???",
     "CPY", "CMP", "???", "???", "CPY", "CMP", "DEC", "???", "INY", "CMP", "DEX", "???", "CPY", "CMP", "DEC", "???",
     "BNE", "CMP", "???", "???", "???", "CMP", "DEC", "???", "CLD", "CMP", "???", "???", "???", "CMP", "DEC", "???",
     "CPX", "SBC", "???", "???", "CPX", "SBC", "INC", "???", "INX", "SBC", "NOP", "???", "CPX", "SBC", "INC", "???",
     "BEQ", "SBC", "???", "???", "???", "SBC", "INC", "???", "SED", "SBC", "???", "???", "???", "SBC", "INC", "???"
};

std::string Utils::ConvertToHex(const uint16_t InInteger)
{
    std::stringstream stream;

    stream << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << InInteger;

    return stream.str();
}

uint8_t Utils::GetHighByte(const uint16_t InValue)
{
    return (InValue >> 8) & 0xFF;
}

uint8_t Utils::GetLowByte(const uint16_t InValue)
{
    return InValue & 0xFF;
}

uint16_t Utils::MakeDword(const uint8_t InLowByte, const uint8_t InHighByte)
{
    return (static_cast<uint16_t>(InHighByte) << 8) | InLowByte;
}

bool Utils::DidCrossPageBoundry(const uint16_t InStartAddress, const uint16_t InEndAddress)
{
    return (InStartAddress & 0xFF00) != (InEndAddress & 0xFF00);
}

std::string Utils::ConvertFlagToString(const ECpuFlag InFlag)
{
    switch (InFlag)
    {
        case ECpuFlag::Carry:            return "C";
        case ECpuFlag::Zero:             return "Z";
        case ECpuFlag::InterruptDisable: return "I";
        case ECpuFlag::DecimalMode:      return "D";
        case ECpuFlag::Break:            return "B";
        case ECpuFlag::Unused:           return "-";
        case ECpuFlag::Overflow:         return "V";
        case ECpuFlag::Negative:         return "N";
    }

    return "-";
}

std::string Utils::ConvertOpcodeToString(const uint8_t InOpcode)
{
    return OpCodeStrings[InOpcode];
}

std::string Utils::LogInstruction(const OpCode InOpCode, const CpuRegisters CpuStateBefore, const uint8_t InTakenCycles)
{
    static uint64_t totalCycles = 0;
    totalCycles += InTakenCycles;

    std::stringstream ss;

    ss << std::left << std::setw(6) << Utils::ConvertToHex(CpuStateBefore.ProgramCounter);

    Bus* dataBus = Bus::Get();

    for (uint16_t i = 0; i < InOpCode.Size; i++)
    {
        const uint8_t value = dataBus->ReadData(CpuStateBefore.ProgramCounter + i);
        ss << std::left << std::setw(3) << Utils::ConvertToHex(value);
    }

    // TODO: fix using 'setw()'
    if (InOpCode.Size == 1)
    {
        ss << "       ";
    }
    else if (InOpCode.Size == 2)
    {
        ss << "    ";
    }
    else
    {
        ss << " ";
    }

    const uint8_t opcodeHex = dataBus->ReadData(CpuStateBefore.ProgramCounter);
    ss << std::left << std::setw(30) << ConvertOpcodeToString(opcodeHex);

    if (InOpCode.AddressingMode != EAddressingMode::Implied)
    {
        // TODO: Print the ASM representation of the operation, taking into account the addressing mode so it's formatted correctly.
    }

    ss << "A:" << ConvertToHex(CpuStateBefore.Accumulator) << " ";
    ss << "X:" << ConvertToHex(CpuStateBefore.X) << " ";
    ss << "Y:" << ConvertToHex(CpuStateBefore.Y) << " ";
    ss << "P:" << ConvertToHex(CpuStateBefore.GetFlags()) << " ";
    ss << "SP:" << ConvertToHex(CpuStateBefore.StackPointer) << " ";
    ss << "PPU: " << "0,0" << " "; // TODO: PPU hasn't been implemented yet so this will always be wrong when compared to the example log.
    ss << "CYC:" << totalCycles;   // TODO: Cycle accurate timing hasn't been implemented yet so this will always be wrong when compared to the example log.

    std::cout << ss.str() << std::endl;
    return ss.str();
}