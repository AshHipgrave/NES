#pragma once

/**
 * Represents the different interrupt types available on a 6502 CPU.
 * Depending on the interrupt that was triggered will depend on how the CPU handles it.
 */
enum class EInterruptType : uint8_t
{
    /**
    * Non-Maskable Interrupt.
    */
    NMI,

    /**
    * An interrupt triggered by the BRK instruction.
    */
    BRK,

    /**
    * Interrupt request. Can be ignored if the 'InterruptDisable' CPU flag is set.
    */
    IRQ
};