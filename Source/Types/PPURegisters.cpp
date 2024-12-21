#include "pch.h"
#include "Types/PPURegisters.h"
#include "Enums/PPUFlags.h"

uint8_t PPURegisters::GetMaskFlags() const
{
    return static_cast<uint8_t>(Mask.to_ulong());
}

uint8_t PPURegisters::GetStatusFlags() const
{
    return static_cast<uint8_t>(Status.to_ulong());
}

uint8_t PPURegisters::GetControlFlags() const
{
    return static_cast<uint8_t>(Control.to_ulong());
}