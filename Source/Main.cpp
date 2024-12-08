#include "pch.h"
#include "Core/Core.h"
#include "Emulator.h"

int main(int argc, char* argv[])
{
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);

    Emulator emulator;
    emulator.Run();

    return 0;
}