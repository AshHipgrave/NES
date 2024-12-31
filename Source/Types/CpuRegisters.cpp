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
    const std::string x = std::format("{:02X}", X);
    const std::string y = std::format("{:02X}", Y);

    const std::string accumulator = std::format("{:02X}", Accumulator);
    const std::string stackPointer = std::format("{:02X}", StackPointer);

    const std::string programRegister = std::format("{:02X}", GetFlags());

    return std::format("A:{} X:{} Y:{} P:{} SP:{}", accumulator, x, y, programRegister, stackPointer);
}