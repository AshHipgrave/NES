#pragma once

#include "Enums/AddressingMode.h"

class Cpu;

/**
 * Represents a single opcode supported by the 6502
 */
struct OpCode
{
    /**
     * Assembly code representation of this opcode.
     */
    std::string AssemblyCodeString = "";

    /**
    * The size of this opcode. This value is used to increment the program counter when the opcode executes.
    * Some opcodes may increment by more than this value (e.g. if they crossed a page boundry).
    */
    uint8_t Size = 0;

    /**
     * The number of cycles this opcode takes. This will be used to calculate the correct timing for the CPU.
     */
    uint8_t CycleCount = 0;

    /**
     * The addressing mode used by this opcode. This will be used to retrieve the correct value from RAM for the CPU to operate on.
     */
    EAddressingMode AddressingMode;

    /**
     * Returns the assembly code string representation of this opcode.
     */
    std::string ToString() const
    {
        return AssemblyCodeString;
    }
};

/**
 * Represents an instruction that the emulator can execute.
 * An instruction is a combination of opcodes and a function pointer to the function that will execute this opcode.
 */
struct Instruction
{
    /**
     * The opcode for this instruction.
     */
    OpCode Code;

    /**
     * Function pointer to the function that will execute this instruction.
     */
    uint8_t(Cpu::* PFN_OpCodeHandlerFunction)(const OpCode&) = nullptr;
};

/// 
/// Load Accumulator
/// 

const OpCode LDA_Immediate = { "LDA", 2, 2, EAddressingMode::Immediate };
const OpCode LDA_ZeroPage  = { "LDA", 2, 3, EAddressingMode::ZeroPage  };
const OpCode LDA_ZeroPageX = { "LDA", 2, 4, EAddressingMode::ZeroPageX };
const OpCode LDA_Absolute  = { "LDA", 3, 4, EAddressingMode::Absolute  };
const OpCode LDA_AbsoluteX = { "LDA", 3, 4, EAddressingMode::AbsoluteX };
const OpCode LDA_AbsoluteY = { "LDA", 3, 4, EAddressingMode::AbsoluteY };
const OpCode LDA_IndirectX = { "LDA", 2, 6, EAddressingMode::IndirectX };
const OpCode LDA_IndirectY = { "LDA", 2, 5, EAddressingMode::IndirectY };

/// 
/// Load X Register
/// 

const OpCode LDX_Immediate = { "LDX", 2, 2, EAddressingMode::Immediate };
const OpCode LDX_ZeroPage  = { "LDX", 2, 3, EAddressingMode::ZeroPage  };
const OpCode LDX_ZeroPageY = { "LDX", 2, 4, EAddressingMode::ZeroPageY };
const OpCode LDX_Absolute  = { "LDX", 3, 4, EAddressingMode::Absolute  };
const OpCode LDX_AbsoluteY = { "LDX", 3, 4, EAddressingMode::AbsoluteY };

/// 
/// Load Y Register
/// 

const OpCode LDY_Immediate = { "LDY", 2, 2, EAddressingMode::Immediate };
const OpCode LDY_ZeroPage  = { "LDY", 2, 3, EAddressingMode::ZeroPage  };
const OpCode LDY_ZeroPageX = { "LDY", 2, 4, EAddressingMode::ZeroPageX };
const OpCode LDY_Absolute  = { "LDY", 3, 4, EAddressingMode::Absolute  };
const OpCode LDY_AbsoluteX = { "LDY", 3, 4, EAddressingMode::AbsoluteX };

/// 
/// Store Accumulator
/// 

const OpCode STA_ZeroPage  = { "STA", 2, 3, EAddressingMode::ZeroPage  };
const OpCode STA_ZeroPageX = { "STA", 2, 4, EAddressingMode::ZeroPageX };
const OpCode STA_Absolute  = { "STA", 3, 4, EAddressingMode::Absolute  };
const OpCode STA_AbsoluteX = { "STA", 3, 5, EAddressingMode::AbsoluteX };
const OpCode STA_AbsoluteY = { "STA", 3, 5, EAddressingMode::AbsoluteY };
const OpCode STA_IndirectX = { "STA", 2, 6, EAddressingMode::IndirectX };
const OpCode STA_IndirectY = { "STA", 2, 6, EAddressingMode::IndirectY };

/// 
/// Store X Register
/// 

const OpCode STX_ZeroPage  = { "STX", 2, 3, EAddressingMode::ZeroPage  };
const OpCode STX_ZeroPageY = { "STX", 2, 4, EAddressingMode::ZeroPageY };
const OpCode STX_Absolute  = { "STX", 3, 4, EAddressingMode::Absolute  };

/// 
/// Store Y Register
/// 

const OpCode STY_ZeroPage  = { "STY", 2, 3, EAddressingMode::ZeroPage  };
const OpCode STY_ZeroPageX = { "STY", 2, 4, EAddressingMode::ZeroPageX };
const OpCode STY_Absolute  = { "STY", 3, 4, EAddressingMode::Absolute  };

///
/// Transfer Register
///

const OpCode TAX_Implied = { "TAX", 1, 2, EAddressingMode::Implied };
const OpCode TAY_Implied = { "TAY", 1, 2, EAddressingMode::Implied };
const OpCode TXA_Implied = { "TXA", 1, 2, EAddressingMode::Implied };
const OpCode TYA_Implied = { "TYA", 1, 2, EAddressingMode::Implied };

///
/// Stack Operations
///

const OpCode TSX_Implied = { "TSX", 1, 2, EAddressingMode::Implied };
const OpCode TXS_Implied = { "TXS", 1, 2, EAddressingMode::Implied };
const OpCode PHA_Implied = { "PHA", 1, 3, EAddressingMode::Implied };
const OpCode PHP_Implied = { "PHP", 1, 3, EAddressingMode::Implied };
const OpCode PLA_Implied = { "PLA", 1, 4, EAddressingMode::Implied };
const OpCode PLP_Implied = { "PLP", 1, 4, EAddressingMode::Implied };

///
/// Logical AND
///

const OpCode AND_Immediate = { "AND", 2, 2, EAddressingMode::Immediate };
const OpCode AND_ZeroPage  = { "AND", 2, 3, EAddressingMode::ZeroPage  };
const OpCode AND_ZeroPageX = { "AND", 2, 4, EAddressingMode::ZeroPageX };
const OpCode AND_Absolute  = { "AND", 3, 4, EAddressingMode::Absolute  };
const OpCode AND_AbsoluteX = { "AND", 3, 4, EAddressingMode::AbsoluteX };
const OpCode AND_AbsoluteY = { "AND", 3, 4, EAddressingMode::AbsoluteY };
const OpCode AND_IndirectX = { "AND", 2, 6, EAddressingMode::IndirectX };
const OpCode AND_IndirectY = { "AND", 2, 5, EAddressingMode::IndirectY };

///
/// Logical EOR (Exclusive OR)
///

const OpCode EOR_Immediate = { "EOR", 2, 2, EAddressingMode::Immediate };
const OpCode EOR_ZeroPage  = { "EOR", 2, 3, EAddressingMode::ZeroPage  };
const OpCode EOR_ZeroPageX = { "EOR", 2, 4, EAddressingMode::ZeroPageX };
const OpCode EOR_Absolute  = { "EOR", 3, 4, EAddressingMode::Absolute  };
const OpCode EOR_AbsoluteX = { "EOR", 3, 4, EAddressingMode::AbsoluteX };
const OpCode EOR_AbsoluteY = { "EOR", 3, 4, EAddressingMode::AbsoluteY };
const OpCode EOR_IndirectX = { "EOR", 2, 6, EAddressingMode::IndirectX };
const OpCode EOR_IndirectY = { "EOR", 2, 5, EAddressingMode::IndirectY };

///
/// Logical ORA (Inclusive OR)
///

const OpCode ORA_Immediate = { "ORA", 2, 2, EAddressingMode::Immediate };
const OpCode ORA_ZeroPage  = { "ORA", 2, 3, EAddressingMode::ZeroPage  };
const OpCode ORA_ZeroPageX = { "ORA", 2, 4, EAddressingMode::ZeroPageX };
const OpCode ORA_Absolute  = { "ORA", 3, 4, EAddressingMode::Absolute  };
const OpCode ORA_AbsoluteX = { "ORA", 3, 4, EAddressingMode::AbsoluteX };
const OpCode ORA_AbsoluteY = { "ORA", 3, 4, EAddressingMode::AbsoluteY };
const OpCode ORA_IndirectX = { "ORA", 2, 6, EAddressingMode::IndirectX };
const OpCode ORA_IndirectY = { "ORA", 2, 5, EAddressingMode::IndirectY };

///
/// Bit Test 
///

const OpCode BIT_ZeroPage = { "BIT", 2, 3, EAddressingMode::ZeroPage };
const OpCode BIT_Absolute = { "BIT", 3, 4, EAddressingMode::Absolute };

///
/// Add With Carry 
///

const OpCode ADC_Immediate = { "ADC", 2, 2, EAddressingMode::Immediate };
const OpCode ADC_ZeroPage  = { "ADC", 2, 3, EAddressingMode::ZeroPage  };
const OpCode ADC_ZeroPageX = { "ADC", 2, 4, EAddressingMode::ZeroPageX };
const OpCode ADC_Absolute  = { "ADC", 3, 4, EAddressingMode::Absolute  };
const OpCode ADC_AbsoluteX = { "ADC", 3, 4, EAddressingMode::AbsoluteX };
const OpCode ADC_AbsoluteY = { "ADC", 3, 4, EAddressingMode::AbsoluteY };
const OpCode ADC_IndirectX = { "ADC", 2, 6, EAddressingMode::IndirectX };
const OpCode ADC_IndirectY = { "ADC", 2, 5, EAddressingMode::IndirectY };

///                                
/// Subtract With Carry            
///                                

const OpCode SBC_Immediate = { "SBC", 2, 2, EAddressingMode::Immediate };
const OpCode SBC_ZeroPage  = { "SBC", 2, 3, EAddressingMode::ZeroPage  };
const OpCode SBC_ZeroPageX = { "SBC", 2, 4, EAddressingMode::ZeroPageX };
const OpCode SBC_Absolute  = { "SBC", 3, 4, EAddressingMode::Absolute  };
const OpCode SBC_AbsoluteX = { "SBC", 3, 4, EAddressingMode::AbsoluteX };
const OpCode SBC_AbsoluteY = { "SBC", 3, 4, EAddressingMode::AbsoluteY };
const OpCode SBC_IndirectX = { "SBC", 2, 6, EAddressingMode::IndirectX };
const OpCode SBC_IndirectY = { "SBC", 2, 5, EAddressingMode::IndirectY };

///                                
/// Compare                        
///                                

const OpCode CMP_Immediate = { "CMP", 2, 2, EAddressingMode::Immediate };
const OpCode CMP_ZeroPage  = { "CMP", 2, 3, EAddressingMode::ZeroPage  };
const OpCode CMP_ZeroPageX = { "CMP", 2, 4, EAddressingMode::ZeroPageX };
const OpCode CMP_Absolute  = { "CMP", 3, 4, EAddressingMode::Absolute  };
const OpCode CMP_AbsoluteX = { "CMP", 3, 4, EAddressingMode::AbsoluteX };
const OpCode CMP_AbsoluteY = { "CMP", 3, 4, EAddressingMode::AbsoluteY };
const OpCode CMP_IndirectX = { "CMP", 2, 6, EAddressingMode::IndirectX };
const OpCode CMP_IndirectY = { "CMP", 2, 5, EAddressingMode::IndirectY };

///                                
/// Compare X Register             
///                                

const OpCode CPX_Immediate = { "CPX", 2, 2, EAddressingMode::Immediate };
const OpCode CPX_ZeroPage  = { "CPX", 2, 3, EAddressingMode::ZeroPage  };
const OpCode CPX_Absolute  = { "CPX", 3, 4, EAddressingMode::Absolute  };

///                                
/// Compare Y Register             
///                                

const OpCode CPY_Immediate = { "CPY", 2, 2, EAddressingMode::Immediate };
const OpCode CPY_ZeroPage  = { "CPY", 2, 3, EAddressingMode::ZeroPage  };
const OpCode CPY_Absolute  = { "CPY", 3, 4, EAddressingMode::Absolute  };

///                                
/// Increment Memory Location      
///                                

const OpCode INC_ZeroPage  = { "INC", 2, 5, EAddressingMode::ZeroPage  };
const OpCode INC_ZeroPageX = { "INC", 2, 6, EAddressingMode::ZeroPageX };
const OpCode INC_Absolute  = { "INC", 3, 6, EAddressingMode::Absolute  };
const OpCode INC_AbsoluteX = { "INC", 3, 7, EAddressingMode::AbsoluteX };

///
/// Increment Register
/// 

const OpCode INX_Implied = { "INX", 1, 2, EAddressingMode::Implied };
const OpCode INY_Implied = { "INY", 1, 2, EAddressingMode::Implied };

///
/// Decrement Memory Location
///

const OpCode DEC_ZeroPage  = { "DEC", 2, 5, EAddressingMode::ZeroPage  };
const OpCode DEC_ZeroPageX = { "DEC", 2, 6, EAddressingMode::ZeroPageX };
const OpCode DEC_Absolute  = { "DEC", 3, 6, EAddressingMode::Absolute  };
const OpCode DEC_AbsoluteX = { "DEC", 3, 7, EAddressingMode::AbsoluteX };

///
/// Decrement Register
/// 

const OpCode DEX_Implied = { "DEX", 1, 2, EAddressingMode::Implied };
const OpCode DEY_Implied = { "DEY", 1, 2, EAddressingMode::Implied };

///
/// Arithmetic Shift Left
///

const OpCode ASL_Accumulator = { "ASL", 1, 2, EAddressingMode::Accumulator };
const OpCode ASL_ZeroPage    = { "ASL", 2, 5, EAddressingMode::ZeroPage    };
const OpCode ASL_ZeroPageX   = { "ASL", 2, 6, EAddressingMode::ZeroPageX   };
const OpCode ASL_Absolute    = { "ASL", 3, 6, EAddressingMode::Absolute    };
const OpCode ASL_AbsoluteX   = { "ASL", 3, 7, EAddressingMode::AbsoluteX   };

///
/// Logical Shift Right
///

const OpCode LSR_Accumulator = { "LSR", 1, 2, EAddressingMode::Accumulator };
const OpCode LSR_ZeroPage    = { "LSR", 2, 5, EAddressingMode::ZeroPage    };
const OpCode LSR_ZeroPageX   = { "LSR", 2, 6, EAddressingMode::ZeroPageX   };
const OpCode LSR_Absolute    = { "LSR", 3, 6, EAddressingMode::Absolute    };
const OpCode LSR_AbsoluteX   = { "LSR", 3, 7, EAddressingMode::AbsoluteX   };

///
/// Rotate Left
///

const OpCode ROL_Accumulator = { "ROL", 1, 2, EAddressingMode::Accumulator };
const OpCode ROL_ZeroPage    = { "ROL", 2, 5, EAddressingMode::ZeroPage    };
const OpCode ROL_ZeroPageX   = { "ROL", 2, 6, EAddressingMode::ZeroPageX   };
const OpCode ROL_Absolute    = { "ROL", 3, 6, EAddressingMode::Absolute    };
const OpCode ROL_AbsoluteX   = { "ROL", 3, 7, EAddressingMode::AbsoluteX   };

///
/// Rotate Right
///

const OpCode ROR_Accumulator = { "ROR", 1, 2, EAddressingMode::Accumulator };
const OpCode ROR_ZeroPage    = { "ROR", 2, 5, EAddressingMode::ZeroPage    };
const OpCode ROR_ZeroPageX   = { "ROR", 2, 6, EAddressingMode::ZeroPageX   };
const OpCode ROR_Absolute    = { "ROR", 3, 6, EAddressingMode::Absolute    };
const OpCode ROR_AbsoluteX   = { "ROR", 3, 7, EAddressingMode::AbsoluteX   };

///
/// Jumps
/// 

const OpCode JMP_Absolute = { "JMP", 3, 3, EAddressingMode::Absolute };
const OpCode JMP_Indirect = { "JMP", 3, 5, EAddressingMode::Indirect };
const OpCode JSR_Absolute = { "JSR", 3, 6, EAddressingMode::Absolute };
const OpCode RTS_Implied  = { "RTS", 1, 6, EAddressingMode::Implied  };

///                               
/// Branches                      
///                               

const OpCode BCC_Relative = { "BCC", 2, 2, EAddressingMode::Relative };
const OpCode BCS_Relative = { "BCS", 2, 2, EAddressingMode::Relative };
const OpCode BEQ_Relative = { "BEQ", 2, 2, EAddressingMode::Relative };
const OpCode BMI_Relative = { "BMI", 2, 2, EAddressingMode::Relative };
const OpCode BNE_Relative = { "BNE", 2, 2, EAddressingMode::Relative };
const OpCode BPL_Relative = { "BPL", 2, 2, EAddressingMode::Relative };
const OpCode BVC_Relative = { "BVC", 2, 2, EAddressingMode::Relative };
const OpCode BVS_Relative = { "BVS", 2, 2, EAddressingMode::Relative };

///
/// Clear Status Flags
/// 

const OpCode CLC_Implied = { "CLC", 1, 2, EAddressingMode::Implied };
const OpCode CLD_Implied = { "CLD", 1, 2, EAddressingMode::Implied };
const OpCode CLI_Implied = { "CLI", 1, 2, EAddressingMode::Implied };
const OpCode CLV_Implied = { "CLV", 1, 2, EAddressingMode::Implied };

///                              
/// Set Status Flags             
///                              

const OpCode SEC_Implied = { "SEC", 1, 2, EAddressingMode::Implied };
const OpCode SED_Implied = { "SED", 1, 2, EAddressingMode::Implied };
const OpCode SEI_Implied = { "SEI", 1, 2, EAddressingMode::Implied };

///                              
/// System Functions             
///                              

const OpCode BRK_Implied = { "BRK", 1, 7, EAddressingMode::Implied };
const OpCode NOP_Implied = { "NOP", 1, 2, EAddressingMode::Implied };
const OpCode RTI_Implied = { "RTI", 1, 6, EAddressingMode::Implied };

///
/// Illegal DCP
/// 

const OpCode DCP_ZeroPage  = { "*DCP", 2, 5, EAddressingMode::ZeroPage  };
const OpCode DCP_ZeroPageX = { "*DCP", 2, 6, EAddressingMode::ZeroPageX };
const OpCode DCP_Absolute  = { "*DCP", 3, 6, EAddressingMode::Absolute  };
const OpCode DCP_AbsoluteX = { "*DCP", 3, 7, EAddressingMode::AbsoluteX };
const OpCode DCP_AbsoluteY = { "*DCP", 3, 7, EAddressingMode::AbsoluteY };
const OpCode DCP_IndirectX = { "*DCP", 2, 8, EAddressingMode::IndirectX };
const OpCode DCP_IndirectY = { "*DCP", 2, 8, EAddressingMode::IndirectY };

///
/// Illegal ISB/ISC
/// 

const OpCode ISB_ZeroPage  = { "*ISB", 2, 5, EAddressingMode::ZeroPage  };
const OpCode ISB_ZeroPageX = { "*ISB", 2, 6, EAddressingMode::ZeroPageX };
const OpCode ISB_Absolute  = { "*ISB", 3, 6, EAddressingMode::Absolute  };
const OpCode ISB_AbsoluteX = { "*ISB", 3, 7, EAddressingMode::AbsoluteX };
const OpCode ISB_AbsoluteY = { "*ISB", 3, 7, EAddressingMode::AbsoluteY };
const OpCode ISB_IndirectX = { "*ISB", 2, 8, EAddressingMode::IndirectX };
const OpCode ISB_IndirectY = { "*ISB", 2, 8, EAddressingMode::IndirectY };

///
/// Illegal LAX
///

const OpCode LAX_ZeroPage  = { "*LAX", 2, 3, EAddressingMode::ZeroPage  };
const OpCode LAX_ZeroPageY = { "*LAX", 2, 4, EAddressingMode::ZeroPageY };
const OpCode LAX_Absolute  = { "*LAX", 3, 4, EAddressingMode::Absolute  };
const OpCode LAX_AbsoluteY = { "*LAX", 3, 4, EAddressingMode::AbsoluteY };
const OpCode LAX_IndirectX = { "*LAX", 2, 6, EAddressingMode::IndirectX };
const OpCode LAX_IndirectY = { "*LAX", 2, 5, EAddressingMode::IndirectY };

///
/// Illegal RLA
/// 

const OpCode RLA_ZeroPage  = { "*RLA", 2, 5, EAddressingMode::ZeroPage  };
const OpCode RLA_ZeroPageX = { "*RLA", 2, 6, EAddressingMode::ZeroPageX };
const OpCode RLA_Absolute  = { "*RLA", 3, 6, EAddressingMode::Absolute  };
const OpCode RLA_AbsoluteX = { "*RLA", 3, 7, EAddressingMode::AbsoluteX };
const OpCode RLA_AbsoluteY = { "*RLA", 3, 7, EAddressingMode::AbsoluteY };
const OpCode RLA_IndirectX = { "*RLA", 2, 8, EAddressingMode::IndirectX };
const OpCode RLA_IndirectY = { "*RLA", 2, 8, EAddressingMode::IndirectY };

///
/// Illegal RRA
/// 

const OpCode RRA_ZeroPage  = { "*RRA", 2, 5, EAddressingMode::ZeroPage  };
const OpCode RRA_ZeroPageX = { "*RRA", 2, 6, EAddressingMode::ZeroPageX };
const OpCode RRA_Absolute  = { "*RRA", 3, 6, EAddressingMode::Absolute  };
const OpCode RRA_AbsoluteX = { "*RRA", 3, 7, EAddressingMode::AbsoluteX };
const OpCode RRA_AbsoluteY = { "*RRA", 3, 7, EAddressingMode::AbsoluteY };
const OpCode RRA_IndirectX = { "*RRA", 2, 8, EAddressingMode::IndirectX };
const OpCode RRA_IndirectY = { "*RRA", 2, 8, EAddressingMode::IndirectY };

///
/// Illegal SAX
/// 

const OpCode SAX_ZeroPage  = { "*SAX", 2, 3, EAddressingMode::ZeroPage  };
const OpCode SAX_ZeroPageY = { "*SAX", 2, 4, EAddressingMode::ZeroPageY };
const OpCode SAX_Absolute  = { "*SAX", 3, 4, EAddressingMode::Absolute  };
const OpCode SAX_IndirectX = { "*SAX", 2, 6, EAddressingMode::IndirectX };

///
/// Illegal SLO
/// 

const OpCode SLO_ZeroPage  = { "*SLO", 2, 5, EAddressingMode::ZeroPage  };
const OpCode SLO_ZeroPageX = { "*SLO", 2, 6, EAddressingMode::ZeroPageX };
const OpCode SLO_Absolute  = { "*SLO", 3, 6, EAddressingMode::Absolute  };
const OpCode SLO_AbsoluteX = { "*SLO", 3, 7, EAddressingMode::AbsoluteX };
const OpCode SLO_AbsoluteY = { "*SLO", 3, 7, EAddressingMode::AbsoluteY };
const OpCode SLO_IndirectX = { "*SLO", 2, 8, EAddressingMode::IndirectX };
const OpCode SLO_IndirectY = { "*SLO", 2, 8, EAddressingMode::IndirectY };

///
/// Illegal SRE
/// 

const OpCode SRE_ZeroPage  = { "*SRE", 2, 5, EAddressingMode::ZeroPage  };
const OpCode SRE_ZeroPageX = { "*SRE", 2, 6, EAddressingMode::ZeroPageX };
const OpCode SRE_Absolute  = { "*SRE", 3, 6, EAddressingMode::Absolute  };
const OpCode SRE_AbsoluteX = { "*SRE", 3, 7, EAddressingMode::AbsoluteX };
const OpCode SRE_AbsoluteY = { "*SRE", 3, 7, EAddressingMode::AbsoluteY };
const OpCode SRE_IndirectX = { "*SRE", 2, 8, EAddressingMode::IndirectX };
const OpCode SRE_IndirectY = { "*SRE", 2, 8, EAddressingMode::IndirectY };

///
/// Illegal NOP
///

const OpCode NOP_Immediate = { "*NOP", 2, 2, EAddressingMode::Immediate };
const OpCode NOP_ZeroPage  = { "*NOP", 2, 3, EAddressingMode::ZeroPage  };
const OpCode NOP_ZeroPageX = { "*NOP", 2, 4, EAddressingMode::ZeroPageX };
const OpCode NOP_Absolute  = { "*NOP", 3, 4, EAddressingMode::Absolute  };
const OpCode NOP_AbsoluteX = { "*NOP", 3, 4, EAddressingMode::AbsoluteX };

///
/// Unimplemented opcodes
///

const OpCode OP_NotImplemented = { "???", 0, 0, EAddressingMode::Implied };