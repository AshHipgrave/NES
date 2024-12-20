#pragma once

enum class EMirrorMode : uint8_t;

class Cartridge
{
    friend class UIManager;

public:
    Cartridge();
    ~Cartridge();

    static Cartridge* LoadFromFile(const std::string& InFileName);

    uint8_t ReadProgramData(const uint16_t InAddress) const;
    uint8_t ReadCharacterData(const uint16_t InAddress) const;

    EMirrorMode GetMirrorMode() const;

private:
    uint8_t m_MapperID = 0;
    EMirrorMode m_MirrorMode;

    std::vector<uint8_t> m_ProgramROM;
    std::vector<uint8_t> m_CharacterROM;
};