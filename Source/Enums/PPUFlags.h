#pragma once

/**
 * Represents the status flags for the PPU's status register. Note that only the highest 3 bits are used. 
 * The remaining bits are be used for copy protection on the 2C05 variant of the PPU chip. 
 * For all other variants these bits are set to whatever was last on the data bus (?-TODO: Confirm if this is correct or not).
 */
enum class EPPUStatusFlag : uint8_t
{
    /**
    * Set at the start of VBlank (scanline 241 dot 1) and cleared again on the first pre-render scanline. 
    * Note: Reading the status register will clear this flag.
    */
    VBlank = 7,

    /**
    * Used to detect pixel-perfect collision between the first sprite in OAM (sprite 0) and the background. 
    * The sprite 0 hit flag is immediately set when any opaque pixel of sprite 0 overlaps any opaque pixel of the background, regardless of sprite priority.
    * This flag remains set until dot 1 of the pre-render scanline, so it can only detect 1 collision per-frame.
    */
    SpriteZeroHit = 6,

    /**
    * Intended to be set any time there are more than 8 sprites on a scanline.
    * Due to a bug in the implementation however this can produce false positives and false negatives.
    * Similar to the 'SpriteZeroHit' flag, this flag remains set until the start of the pre-render scanline so it can only be set once per frame.
    */
    SpriteOverflow = 5
};

/**
 * Represents the control flags used by the PPU's Control register.
 * Used to control a mix of render and scroll related settings.
 */
enum class EPPUControlFlags : uint8_t
{
    /**
    * X Scroll position bit 8.
    * For games without scrolling this is the low bit of the base nametable address (usually always 0x00 (?-TODO: Confirm if this is correct)).
    */
    ScrollX                       = 0,

    /**
    * Y Scroll position bit 8.
    * For games without scrolling this is the high bit of the base nametable address (0 = 2000, 1 = 2400, 2 = 2800, 3 = 2C000).
    */
    ScrollY                       = 1,

    /**
    * Controls how much to increment the PPUADDR register whenever the CPU reads/writes to PPUDATA.
    * 0 = Increment by 1 (going across)
    * 1 = Increment by 32 (going down)
    */
    VRAMIncrement                 = 2,

    /**
    * Address of the sprite pattern table. 0 = 0x0000, 1 = 0x1000.
    * Note: this is ignored when in 8x16 mode (aka 'SpriteSize' flag is 1)
    */
    SpritePatternTableAddress     = 3,
    
    /**
    * Address of the background pattern table. 0 = 0x0000, 1 = 0x1000.
    */
    BackgroundPatternTableAddress = 4,

    /**
    * Controls the size of each sprite.
    * 0 = 8x8
    * 1 = 8x16
    */
    SpriteSize                    = 5,

    /**
    * Never set on stock PPU's. When this bit is clear (the usual case), the PPU gets the palette index for the backdrop color from the EXT pins. 
    * The stock NES grounds these pins, making palette index 0 the backdrop color as expected. A secondary picture generator connected to the EXT pins would 
    * be able to replace the backdrop with a different image using colors from the background palette, which could be used for features such as parallax scrolling.
    */
    MasterSlaveSelect             = 6,

    /**
    * When enabled this causes an NMI to be triggered at the start of VBlank.
    * Note: If this flag is flipped from Disabled to Enabled during VBlank (i.e. when the VBlank flag is 1 in the PPUStatus register), then an NMI will be triggered immediately.
    */
    VBlankNMIEnable               = 7
};

/**
 * Represents the status flags for the PPU's Mask register.
 * These flags are used to control the rendering of sprites and backgrounds as well as colour effects.
 */
enum class EPPUMaskFlags : uint8_t
{
    /**
    * When enabled sprites are rendered in greyscale. If cear then sprites are rendered in colour
    */
    RenderGreyscale           = 0,

    /**
    * When enabled the any background pixels in the left most 8 pixels of the screen are drawn. If clear then these pixels are hidden.
    */
    ShowBackgroundLeftMost    = 1,

    /**
    * When enabled the any sprite pixels in the left most 8 pixels of the screen are drawn. If clear then these pixels are hidden.
    */
    ShowSpritesLeftMode       = 2,

    /*
    * When enabled the background is rendered. When clear the background is not rendered.
    */
    EnableBackgroundRendering = 3,

    /*
    * When enabled sprites are rendered. When clear the sprites are not rendered.
    */
    EnableSpriteRendering     = 4,

    /**
    * When enabled Red colours are emphasized.
    * Note: On PAL/Dendy displays this emphasizes Green.
    */
    EmphasizeRed              = 5,

    /**
    * When enabled Green colours are emphasized.
    * Note: On PAL/Dendy displays this emphasizes Red.
    */
    EmphasizeGreen            = 6,

    /**
    * When enabled Blue colours are emphasized.
    */
    EmphasizeBlue             = 7
};