#include "pch.h"
#include "Core/Utils.h"
#include "Core/Core.h"
#include "System/Bus.h"
#include "System/PPU.h"
#include "Types/OpCodes.h"
#include "Enums/CpuFlags.h"
#include "Types/CpuRegisters.h"
#include "Enums/AddressingMode.h"

std::string Utils::ConvertToHexString(const uint16_t InInteger)
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

uint16_t Utils::MakeWord(const uint8_t InLowByte, const uint8_t InHighByte)
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

std::string Utils::LogInstruction(const OpCode InOpCode, const CpuRegisters InCpuStateBefore, const uint64_t InTotalCycles)
{
    std::string output;

    const Bus* dataBus = Bus::Get();
    //const PPU* ppu = dataBus->GetPPU();

    std::stringstream ss;

    ss << std::left << std::setw(6) << Utils::ConvertToHexString(InCpuStateBefore.ProgramCounter);

    for (uint16_t i = 0; i < InOpCode.Size; i++)
    {
        const uint8_t value = dataBus->ReadData(InCpuStateBefore.ProgramCounter + i);
        ss << std::left << std::setw(3) << Utils::ConvertToHexString(value);
    }

    /*
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
    */

    //ss << std::left << std::setw() << InOpCode.ToString();

    ss << " " << DecompileInstruction(InOpCode, InCpuStateBefore) << " ";

    ss << "A:" << ConvertToHexString(InCpuStateBefore.Accumulator) << " ";
    ss << "X:" << ConvertToHexString(InCpuStateBefore.X) << " ";
    ss << "Y:" << ConvertToHexString(InCpuStateBefore.Y) << " ";
    ss << "P:" << ConvertToHexString(InCpuStateBefore.GetFlags()) << " ";
    ss << "SP:" << ConvertToHexString(InCpuStateBefore.StackPointer) << " ";
    ss << "PPU: " << dataBus->GetPPU()->GetCurrentScanLine() << "," << dataBus->GetPPU()->GetCurrentCycle() << " "; // TODO: PPU hasn't been implemented yet so this will always be wrong when compared to the example log.
    ss << "CYC:" << InTotalCycles;

    std::cout << ss.str() << std::endl;
    return ss.str();
}

std::string Utils::DecompileInstruction(const OpCode InOpCode, const CpuRegisters InRegisters)
{
    const Bus* dataBus = Bus::Get();

    uint8_t opData[3] = { 0, 0 , 0 };
    std::string opDataStr[3] = { "", "", "" };

    for (uint8_t i = 0; i < InOpCode.Size; i++)
    {
        opData[i] = dataBus->ReadData(InRegisters.ProgramCounter + i);
        opDataStr[i] = ConvertToHexString(opData[i]);
    }

    switch (InOpCode.AddressingMode)
    {
        case EAddressingMode::Implied:
        {
            return InOpCode.ToString();
        }
        case EAddressingMode::Accumulator:
        {
            return std::format("{} A", InOpCode.ToString());
        };
        case EAddressingMode::Immediate:
        {
            return std::format("{} #${}", InOpCode.ToString(), opDataStr[1]);
        };
        case EAddressingMode::Relative:
        {
            const uint16_t offsetAddress = InRegisters.ProgramCounter + 1;
            const int8_t offset = dataBus->ReadData(offsetAddress);

            const uint16_t baseProgramCounter = InRegisters.ProgramCounter + 2;
            const uint16_t branchedProgramCounter = baseProgramCounter + offset;

            const std::string branchedPCStr = ConvertToHexString(branchedProgramCounter);

            return std::format("{} ${}", InOpCode.ToString(), branchedPCStr);
        }
        case EAddressingMode::ZeroPage:
        {
            const uint16_t address = InRegisters.ProgramCounter + 1;
            const std::string data = ConvertToHexString(dataBus->ReadData(address));

            return std::format("{} ${} = {}", InOpCode.ToString(), opDataStr[1], data);
        };
        case EAddressingMode::ZeroPageX:
        {
            const uint8_t address = (InRegisters.X + opData[1]) & 0xFF;
            const uint8_t data = dataBus->ReadData(address);

            const std::string addressStr = ConvertToHexString(address);
            const std::string dataStr = ConvertToHexString(data);

            return std::format("{} ${},X @ {} = {}", InOpCode.ToString(), opDataStr[1], addressStr, data);
        };
        case EAddressingMode::ZeroPageY:
        {
            const uint8_t address = (InRegisters.Y + opData[1]) & 0xFF;
            const uint8_t data = dataBus->ReadData(address);

            const std::string addressStr = ConvertToHexString(address);
            const std::string dataStr = ConvertToHexString(data);

            return std::format("{} ${},X @ {} = {}", InOpCode.ToString(), opDataStr[1], addressStr, data);
        };
        case EAddressingMode::IndirectX:
        {
            const uint8_t zpAddress = (InRegisters.X + opData[1]) & 0xFF;
            const std::string zpAddressStr = ConvertToHexString(zpAddress);

            const uint8_t lowbyte = dataBus->ReadData(zpAddress);
            const uint8_t highbyte = dataBus->ReadData((zpAddress + 1) & 0xFF);

            const uint16_t finalAddress = Utils::MakeWord(lowbyte, highbyte);

            const std::string finalAddressStr = ConvertToHexString(finalAddress);
            const std::string dataStr = ConvertToHexString(dataBus->ReadData(finalAddress));

            return std::format("{} (${},X) @ {} = {} = {}", InOpCode.ToString(), opDataStr[1], zpAddressStr, finalAddressStr, dataStr);
        };
        case EAddressingMode::IndirectY:
        {
            const uint8_t zpAddress = (InRegisters.X + opData[1]) & 0xFF;
            const std::string zpAddressStr = ConvertToHexString(zpAddress);

            const uint8_t lowbyte = dataBus->ReadData(zpAddress);
            const uint8_t highbyte = dataBus->ReadData((zpAddress + 1) & 0xFF);

            const uint16_t finalAddress = Utils::MakeWord(lowbyte, highbyte) + InRegisters.Y;

            const std::string finalAddressStr = ConvertToHexString(finalAddress);
            const std::string dataStr = ConvertToHexString(dataBus->ReadData(finalAddress));

            return std::format("{} (${},Y) @ {} = {} = {}", InOpCode.ToString(), opDataStr[1], zpAddressStr, finalAddressStr, dataStr);
        };
        case EAddressingMode::Absolute:
        {
            return std::format("{} ${}{}", InOpCode.ToString(), opDataStr[2], opDataStr[1]);
        };
        case EAddressingMode::Indirect:
        {
            const uint8_t pointerLow = dataBus->ReadData(opData[1]);
            const uint8_t pointerHigh = dataBus->ReadData(opData[2]);

            const uint16_t pointer = MakeWord(pointerLow, pointerHigh);

            const uint8_t lowbyte = dataBus->ReadData(pointer);

            const uint16_t highbyteAddress = (pointer & 0xFF) == 0xFF ? (pointer & 0xFF00) : (pointer + 1);
            const uint8_t highbyte = dataBus->ReadData(highbyteAddress);

            const uint16_t address = MakeWord(lowbyte, highbyte);
            
            const std::string addressStr = ConvertToHexString(address);

            return std::format("{} (${})", InOpCode.ToString(), addressStr);
        };
        case EAddressingMode::AbsoluteX:
        {
            const uint16_t baseAddress = MakeWord(opData[1], opData[2]);
            const uint16_t finalAddress = baseAddress + InRegisters.X;

            const std::string finalAddressStr = ConvertToHexString(finalAddress);

            return std::format("{} ${},X", InOpCode.ToString(), finalAddressStr);
        };
        case EAddressingMode::AbsoluteY:
        {
            const uint16_t baseAddress = MakeWord(opData[1], opData[2]);
            const uint16_t finalAddress = baseAddress + InRegisters.Y;

            const std::string finalAddressStr = ConvertToHexString(finalAddress);

            return std::format("{} ${},Y", InOpCode.ToString(), finalAddressStr);
        };
    }

    return InOpCode.ToString();
}