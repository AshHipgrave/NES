#include "pch.h"
#include "Core/Utils.h"
#include "Core/Types.h"

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
