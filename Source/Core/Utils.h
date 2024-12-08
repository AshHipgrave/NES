#pragma once

namespace Utils
{
    template<typename T>
    bool IsBitSet(const T InValue, const int InPosition)
    {
        return (InValue & (1 << InPosition)) != 0;
    }

    bool DidCrossPageBoundry(const uint16_t InStartAddress, const uint16_t InEndAddress);
}