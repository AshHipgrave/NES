#pragma once

/**
 * Represents the different nametable mirror modes the NES' PPU can use
 */
enum class EMirrorMode : uint8_t
{
    /**
     * Nametable is mirrored vertically in a 64x30 table
     * 
     *  --------------- ---------------
     * |               |               |
     * |               |               |
     * |               |               |
     * |       A       |       B       |
     * |               |               |
     * |               |               |
     *  --------------- ---------------
     * |               |               |
     * |               |               |
     * |               |               |
     * |       A       |       B       |
     * |               |               |
     * |               |               |
     *  ---------------- --------------
     */
    Vertical       = 0,

    /**
     * Nametable is mirrored horizontally in a 32x60 table
     *
     *  --------------- ---------------
     * |               |               |
     * |               |               |
     * |               |               |
     * |       A       |       A       |
     * |               |               |
     * |               |               |
     *  --------------- ---------------
     * |               |               |
     * |               |               |
     * |               |               |
     * |       B       |       B       |
     * |               |               |
     * |               |               |
     *  ---------------- --------------
     */
    Horizontal     = 1,

    /**
     * With additional RAM and/or PPU address mapping present on the cartridge
     * four unique nametables can be addressed through the PPU bus, creating a 64x60 tilemap.
     *
     *  --------------- ---------------
     * |               |               |
     * |               |               |
     * |               |               |
     * |       A       |       B       |
     * |               |               |
     * |               |               |
     *  --------------- ---------------
     * |               |               |
     * |               |               |
     * |               |               |
     * |       C       |       D       |
     * |               |               |
     * |               |               |
     *  ---------------- --------------
     */
    FourScreen     = 2,

    /**
     * Only available with certain mappers resulting in two 32x30 tilemaps
     *
     *  --------------- ---------------
     * |               |               |
     * |               |               |
     * |               |               |
     * |       A       |       A       |
     * |               |               |
     * |               |               |
     *  --------------- ---------------
     * |               |               |
     * |               |               |
     * |               |               |
     * |       A       |       A       |
     * |               |               |
     * |               |               |
     *  ---------------- --------------
     */
    SingleScreen = 3
};