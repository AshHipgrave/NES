#include "pch.h"
#include "Types/CpuRegisters.h"
#include "Enums/CpuFlags.h"
#include "Core/Utils.h"

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

std::string CpuRegisters::ToString() const
{
    const std::string x = Utils::ConvertToHexString(X);
    const std::string y = Utils::ConvertToHexString(Y);

    const std::string accumulator = Utils::ConvertToHexString(Accumulator);
    const std::string stackPointer = Utils::ConvertToHexString(StackPointer);

    const std::string programRegister = Utils::ConvertToHexString(GetFlags());

    return std::format("A:{} X:{} Y:{} P:{} SP:{}", accumulator, x, y, programRegister, stackPointer);
}