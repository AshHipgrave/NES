#pragma once

namespace Utils
{
    template<typename T>
    bool IsBitSet(const T InValue, const int InPosition)
    {
        return (InValue & (1 << (InPosition - 1))) != 0;
    }

    uint8_t GetHighByte(const uint16_t InValue);
    uint8_t GetLowByte(const uint16_t InValue);

    uint16_t Combine(const uint8_t InLowByte, const uint8_t InHighByte);

    bool DidCrossPageBoundry(const uint16_t InStartAddress, const uint16_t InEndAddress);
}