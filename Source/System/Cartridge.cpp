#include "pch.h"
#include "System/Cartridge.h"
#include "Logging/Log.h"

Cartridge::Cartridge()
{

}

Cartridge::~Cartridge()
{

}

Cartridge* Cartridge::LoadFromFile(const std::string& InFileName)
{
    Cartridge* cartridge = new Cartridge();

    std::ifstream romFile(InFileName, std::ios::binary);

    if (!romFile)
    {
        EMULATOR_LOG_ERROR("Failed to load ROM file '{}'", InFileName.c_str());
        return nullptr;
    }

    // Read 'NES' magic header to ensure it's correct
    uint8_t header[16];
    romFile.read((char*)header, 16);

    if (header[0] != 'N' || header[1] != 'E' || header[2] != 'S' || header[3] != 0x1A)
    {
        EMULATOR_LOG_ERROR("Failed to load ROM file '{}', it does not appear to be a valid NES ROM(missing magic header).", InFileName.c_str());
        return nullptr;
    }

    // PRG and CHR sizes
    int programROMSize = header[4] * 16 * 1024; // PRG-ROM in 16KB units
    int charROMSize = header[5] * 8 * 1024;     // CHR-ROM in 8KB units

    // Mapper and mirroring
    cartridge->m_MapperID = (header[6] >> 4) | (header[7] & 0xF0);
    cartridge->m_MirrorMode = static_cast<EMirrorMode>(header[6] & 0x01);

    // Load PRG-ROM
    cartridge->m_ProgramROM.resize(programROMSize);
    romFile.read((char*)cartridge->m_ProgramROM.data(), programROMSize);

    // Load CHR-ROM (optional)
    if (charROMSize > 0)
    {
        cartridge->m_CharacterROM.resize(charROMSize);
        romFile.read((char*)cartridge->m_CharacterROM.data(), charROMSize);
    }

    return cartridge;
}

uint8_t Cartridge::ReadProgramData(const uint16_t InAddress) const
{
    return m_ProgramROM[InAddress];
}

uint8_t Cartridge::ReadCharacterData(const uint16_t InAddress) const
{
    return m_CharacterROM[InAddress];
}

EMirrorMode Cartridge::GetMirrorMode() const
{
    return m_MirrorMode;
}
