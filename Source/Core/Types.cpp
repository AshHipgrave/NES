#include "pch.h"
#include "Core/Types.h"

///
/// CPU 
///

uint8_t CpuRegisters::GetFlags() const
{
    return static_cast<uint8_t>(Flags.to_ulong());
}

void CpuRegisters::SetFlag(const ECpuFlag InFlag, const bool bInShouldSet)
{
    Flags.set(std::size_t(InFlag), bInShouldSet);
}

bool CpuRegisters::IsFlagSet(const ECpuFlag InFlag) const
{
    return Flags.test(std::size_t(InFlag));
}

///
/// PPU 
///

uint8_t PPURegisters::GetFlags() const
{
    return static_cast<uint8_t>(Status.to_ulong());
}

void PPURegisters::SetFlag(const EPPUStatusFlag InFlag, const bool bInShouldSet)
{
    Status.set(std::size_t(InFlag), bInShouldSet);
}

bool PPURegisters::IsFlagSet(const EPPUStatusFlag InFlag) const
{
    return Status.test(std::size_t(InFlag));
}
