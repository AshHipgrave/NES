#include "pch.h"
#include "Core/Utils.h"
#include "Core/Types.h"
#include "System/Bus.h"

/**
 * String representation of all opcodes.
 * Opcodes prefixed with an asterisk are illegal/invalid opcodes.
 */
const std::vector<std::string> OpCodeStrings = 
{
     "BRK",  "ORA", "*JAM", "*SLO", "*NOP", "ORA", "ASL", "*SLO", "PHP", "ORA",  "ASL",  "*ANC",  "*NOP", "ORA", "ASL",  "*SLO",
     "BPL",  "ORA", "*JAM", "*SLO", "*NOP", "ORA", "ASL", "*SLO", "CLC", "ORA",  "*NOP", "*SLO",  "*NOP", "ORA", "ASL",  "*SLO",
     "JSR",  "AND", "*JAM", "*RLA", "BIT",  "AND", "ROL", "*RLA", "PLP", "AND",  "ROL",  "*ANC",  "BIT",  "AND", "ROL",  "*RLA",
     "BMI",  "AND", "*JAM", "*RLA", "*NOP", "AND", "ROL", "*RLA", "SEC", "AND",  "*NOP", "*RLA",  "*NOP", "AND", "ROL",  "*RLA",
     "RTI",  "EOR", "*JAM", "*SRE", "*NOP", "EOR", "LSR", "*SRE", "PHA", "EOR",  "LSR",  "*ALR",  "JMP",  "EOR", "LSR",  "*SRE",
     "BVC",  "EOR", "*JAM", "*SRE", "*NOP", "EOR", "LSR", "*SRE", "CLI", "EOR",  "*NOP", "*SRE",  "*NOP", "EOR", "LSR",  "*SRE",
     "RTS",  "ADC", "*JAM", "*RRA", "*NOP", "ADC", "ROR", "*RRA", "PLA", "ADC",  "ROR",  "*ARR",  "JMP",  "ADC", "ROR",  "*RRA",
     "BVS",  "ADC", "*JAM", "*RRA", "*NOP", "ADC", "ROR", "*RRA", "SEI", "ADC",  "*NOP", "*RRA",  "*NOP", "ADC", "ROR",  "*RRA",
     "*NOP", "STA", "*NOP", "*SAX", "STY",  "STA", "STX", "*SAX", "DEY", "*NOP", "TXA",  "*ANE",  "STY",  "STA", "STX",  "*SAX",
     "BCC",  "STA", "*JAM", "*SHA", "STY",  "STA", "STX", "*SAX", "TYA", "STA",  "TXS",  "*TAS",  "*SHY", "STA", "*SHX", "*SHA",
     "LDY",  "LDA", "LDX",  "*LAX", "LDY",  "LDA", "LDX", "*LAX", "TAY", "LDA",  "TAX",  "*LXA",  "LDY",  "LDA", "LDX",  "*LAX",
     "BCS",  "LDA", "*JAM", "*LAX", "LDY",  "LDA", "LDX", "*LAX", "CLV", "LDA",  "TSX",  "*LAS",  "LDX",  "LDA", "LDX",  "*LAX",
     "CPY",  "CMP", "*NOP", "*DCP", "CPY",  "CMP", "DEC", "*DCP", "INY", "CMP",  "DEX",  "*SBX",  "CPY",  "CMP", "DEC",  "*DCP",
     "BNE",  "CMP", "*JAM", "*DCP", "*NOP", "CMP", "DEC", "*DCP", "CLD", "CMP",  "*NOP", "*DCP",  "*NOP", "CMP", "DEC",  "*DCP",
     "CPX",  "SBC", "*NOP", "*ISC", "CPX",  "SBC", "INC", "*ISC", "INX", "SBC",  "NOP",  "*USBC", "CPX",  "SBC", "INC",  "*ISC",
     "BEQ",  "SBC", "*JAM", "*ISC", "*NOP", "SBC", "INC", "*ISC", "SED", "SBC",  "*NOP", "*ISC",  "*NOP", "SBC", "INC",  "*ISC"
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