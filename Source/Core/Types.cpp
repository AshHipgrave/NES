#include "pch.h"
#include "Core/Types.h"

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
