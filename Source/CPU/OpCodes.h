#pragma once

#include "Core/Types.h"

/// 
/// Load Accumulator
/// 

constexpr OpCode LDA_Immediate = { 2, 2, EAddressingMode::Immediate };
constexpr OpCode LDA_ZeroPage =  { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode LDA_ZeroPageX = { 2, 4, EAddressingMode::ZeroPageX };
constexpr OpCode LDA_Absolute =  { 3, 4, EAddressingMode::Absolute  };
constexpr OpCode LDA_AbsoluteX = { 3, 4, EAddressingMode::AbsoluteX };
constexpr OpCode LDA_AbsoluteY = { 3, 4, EAddressingMode::AbsoluteY };
constexpr OpCode LDA_IndirectX = { 2, 6, EAddressingMode::IndirectX };
constexpr OpCode LDA_IndirectY = { 2, 5, EAddressingMode::IndirectY };

/// 
/// Load X Register
/// 

constexpr OpCode LDX_Immediate = { 2, 2, EAddressingMode::Immediate };
constexpr OpCode LDX_ZeroPage  = { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode LDX_ZeroPageY = { 2, 4, EAddressingMode::ZeroPageY };
constexpr OpCode LDX_Absolute  = { 3, 4, EAddressingMode::Absolute  };
constexpr OpCode LDX_AbsoluteY = { 3, 4, EAddressingMode::AbsoluteY };

/// 
/// Load Y Register
/// 

constexpr OpCode LDY_Immediate = { 2, 2, EAddressingMode::Immediate };
constexpr OpCode LDY_ZeroPage  = { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode LDY_ZeroPageX = { 2, 4, EAddressingMode::ZeroPageX };
constexpr OpCode LDY_Absolute  = { 3, 4, EAddressingMode::Absolute  };
constexpr OpCode LDY_AbsoluteX = { 3, 4, EAddressingMode::AbsoluteX };

/// 
/// Store Accumulator
/// 

constexpr OpCode STA_ZeroPage  = { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode STA_ZeroPageX = { 2, 4, EAddressingMode::ZeroPageX };
constexpr OpCode STA_Absolute  = { 3, 4, EAddressingMode::Absolute  };
constexpr OpCode STA_AbsoluteX = { 3, 5, EAddressingMode::AbsoluteX };
constexpr OpCode STA_AbsoluteY = { 3, 5, EAddressingMode::AbsoluteY };
constexpr OpCode STA_IndirectX = { 2, 6, EAddressingMode::IndirectX };
constexpr OpCode STA_IndirectY = { 2, 6, EAddressingMode::IndirectY };

/// 
/// Store X Register
/// 

constexpr OpCode STX_ZeroPage  = { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode STX_ZeroPageY = { 2, 4, EAddressingMode::ZeroPageY };
constexpr OpCode STX_Absolute =  { 3, 4, EAddressingMode::Absolute  };

/// 
/// Store Y Register
/// 

constexpr OpCode STY_ZeroPage  = { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode STY_ZeroPageX = { 2, 4, EAddressingMode::ZeroPageX };
constexpr OpCode STY_Absolute  = { 3, 4, EAddressingMode::Absolute  };

///
/// Transfer Register
///

constexpr OpCode TAX_Implied = { 1, 2, EAddressingMode::Implied };
constexpr OpCode TAY_Implied = { 1, 2, EAddressingMode::Implied };
constexpr OpCode TXA_Implied = { 1, 2, EAddressingMode::Implied };
constexpr OpCode TYA_Implied = { 1, 2, EAddressingMode::Implied };

///
/// Stack Operations
///

constexpr OpCode TSX_Implied = { 1, 2, EAddressingMode::Implied };
constexpr OpCode TXS_Implied = { 1, 2, EAddressingMode::Implied };
constexpr OpCode PHA_Implied = { 1, 3, EAddressingMode::Implied };
constexpr OpCode PHP_Implied = { 1, 3, EAddressingMode::Implied };
constexpr OpCode PLA_Implied = { 1, 4, EAddressingMode::Implied };
constexpr OpCode PLP_Implied = { 1, 4, EAddressingMode::Implied };

///
/// Logical AND
///

constexpr OpCode AND_Immediate = { 2, 2, EAddressingMode::Immediate };
constexpr OpCode AND_ZeroPage  = { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode AND_ZeroPageX = { 2, 4, EAddressingMode::ZeroPageX };
constexpr OpCode AND_Absolute  = { 3, 4, EAddressingMode::Absolute  };
constexpr OpCode AND_AbsoluteX = { 3, 4, EAddressingMode::AbsoluteX };
constexpr OpCode AND_AbsoluteY = { 3, 4, EAddressingMode::AbsoluteY };
constexpr OpCode AND_IndirectX = { 2, 6, EAddressingMode::IndirectX };
constexpr OpCode AND_IndirectY = { 2, 5, EAddressingMode::IndirectY };

///
/// Logical EOR (Exclusive OR)
///

constexpr OpCode EOR_Immediate = { 2, 2, EAddressingMode::Immediate };
constexpr OpCode EOR_ZeroPage  = { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode EOR_ZeroPageX = { 2, 4, EAddressingMode::ZeroPageX };
constexpr OpCode EOR_Absolute  = { 3, 4, EAddressingMode::Absolute  };
constexpr OpCode EOR_AbsoluteX = { 3, 4, EAddressingMode::AbsoluteX };
constexpr OpCode EOR_AbsoluteY = { 3, 4, EAddressingMode::AbsoluteY };
constexpr OpCode EOR_IndirectX = { 2, 6, EAddressingMode::IndirectX };
constexpr OpCode EOR_IndirectY = { 2, 5, EAddressingMode::IndirectY };

///
/// Logical ORA (Inclusive OR)
///

constexpr OpCode ORA_Immediate = { 2, 2, EAddressingMode::Immediate };
constexpr OpCode ORA_ZeroPage  = { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode ORA_ZeroPageX = { 2, 4, EAddressingMode::ZeroPageX };
constexpr OpCode ORA_Absolute  = { 3, 4, EAddressingMode::Absolute  };
constexpr OpCode ORA_AbsoluteX = { 3, 4, EAddressingMode::AbsoluteX };
constexpr OpCode ORA_AbsoluteY = { 3, 4, EAddressingMode::AbsoluteY };
constexpr OpCode ORA_IndirectX = { 2, 6, EAddressingMode::IndirectX };
constexpr OpCode ORA_IndirectY = { 2, 5, EAddressingMode::IndirectY };

///
/// Bit Test 
///

constexpr OpCode BIT_ZeroPage = { 2, 3, EAddressingMode::ZeroPage };
constexpr OpCode BIT_Absolute = { 3, 4, EAddressingMode::Absolute };

///
/// Add With Carry 
///

constexpr OpCode ADC_Immediate = { 2, 2, EAddressingMode::Immediate };
constexpr OpCode ADC_ZeroPage  = { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode ADC_ZeroPageX = { 2, 4, EAddressingMode::ZeroPageX };
constexpr OpCode ADC_Absolute  = { 3, 4, EAddressingMode::Absolute  };
constexpr OpCode ADC_AbsoluteX = { 3, 4, EAddressingMode::AbsoluteX };
constexpr OpCode ADC_AbsoluteY = { 3, 4, EAddressingMode::AbsoluteY };
constexpr OpCode ADC_IndirectX = { 2, 6, EAddressingMode::IndirectX };
constexpr OpCode ADC_IndirectY = { 2, 5, EAddressingMode::IndirectY };
                                   
///                                
/// Subtract With Carry            
///                                
                                   
constexpr OpCode SBC_Immediate = { 2, 2, EAddressingMode::Immediate };
constexpr OpCode SBC_ZeroPage  = { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode SBC_ZeroPageX = { 2, 4, EAddressingMode::ZeroPageX };
constexpr OpCode SBC_Absolute  = { 3, 4, EAddressingMode::Absolute  };
constexpr OpCode SBC_AbsoluteX = { 3, 4, EAddressingMode::AbsoluteX };
constexpr OpCode SBC_AbsoluteY = { 3, 4, EAddressingMode::AbsoluteY };
constexpr OpCode SBC_IndirectX = { 2, 6, EAddressingMode::IndirectX };
constexpr OpCode SBC_IndirectY = { 2, 5, EAddressingMode::IndirectY };
                                   
///                                
/// Compare                        
///                                
                                   
constexpr OpCode CMP_Immediate = { 2, 2, EAddressingMode::Immediate };
constexpr OpCode CMP_ZeroPage  = { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode CMP_ZeroPageX = { 2, 4, EAddressingMode::ZeroPageX };
constexpr OpCode CMP_Absolute  = { 3, 4, EAddressingMode::Absolute  };
constexpr OpCode CMP_AbsoluteX = { 3, 4, EAddressingMode::AbsoluteX };
constexpr OpCode CMP_AbsoluteY = { 3, 4, EAddressingMode::AbsoluteY };
constexpr OpCode CMP_IndirectX = { 2, 6, EAddressingMode::IndirectX };
constexpr OpCode CMP_IndirectY = { 2, 5, EAddressingMode::IndirectY };
                                   
///                                
/// Compare X Register             
///                                
                                   
constexpr OpCode CPX_Immediate = { 2, 2, EAddressingMode::Immediate };
constexpr OpCode CPX_ZeroPage  = { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode CPX_Absolute  = { 3, 4, EAddressingMode::Absolute  };
                                   
///                                
/// Compare Y Register             
///                                
                                   
constexpr OpCode CPY_Immediate = { 2, 2, EAddressingMode::Immediate };
constexpr OpCode CPY_ZeroPage  = { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode CPY_Absolute  = { 3, 4, EAddressingMode::Absolute  };
                                   
///                                
/// Increment Memory Location      
///                                
                                   
constexpr OpCode INC_ZeroPage  = { 2, 5, EAddressingMode::ZeroPage  };
constexpr OpCode INC_ZeroPageX = { 2, 6, EAddressingMode::ZeroPageX };
constexpr OpCode INC_Absolute  = { 3, 6, EAddressingMode::Absolute  };
constexpr OpCode INC_AbsoluteX = { 3, 7, EAddressingMode::AbsoluteX };

///
/// Increment Register
/// 

constexpr OpCode INX_Implied = { 1, 2, EAddressingMode::Implied };
constexpr OpCode INY_Implied = { 1, 2, EAddressingMode::Implied };

///
/// Decrement Memory Location
///

constexpr OpCode DEC_ZeroPage  = { 2, 5, EAddressingMode::ZeroPage  };
constexpr OpCode DEC_ZeroPageX = { 2, 6, EAddressingMode::ZeroPageX };
constexpr OpCode DEC_Absolute  = { 3, 6, EAddressingMode::Absolute  };
constexpr OpCode DEC_AbsoluteX = { 3, 7, EAddressingMode::AbsoluteX };

///
/// Decrement Register
/// 

constexpr OpCode DEX_Implied = { 1, 2, EAddressingMode::Implied };
constexpr OpCode DEY_Implied = { 1, 2, EAddressingMode::Implied };

///
/// Arithmetic Shift Left
///

constexpr OpCode ASL_Accumulator = { 1, 2, EAddressingMode::Accumulator };
constexpr OpCode ASL_ZeroPage    = { 2, 5, EAddressingMode::ZeroPage    };
constexpr OpCode ASL_ZeroPageX   = { 2, 6, EAddressingMode::ZeroPageX   };
constexpr OpCode ASL_Absolute    = { 3, 6, EAddressingMode::Absolute    };
constexpr OpCode ASL_AbsoluteX   = { 3, 7, EAddressingMode::AbsoluteX   };

///
/// Logical Shift Right
///

constexpr OpCode LSR_Accumulator = { 1, 2, EAddressingMode::Accumulator };
constexpr OpCode LSR_ZeroPage    = { 2, 5, EAddressingMode::ZeroPage    };
constexpr OpCode LSR_ZeroPageX   = { 2, 6, EAddressingMode::ZeroPageX   };
constexpr OpCode LSR_Absolute    = { 3, 6, EAddressingMode::Absolute    };
constexpr OpCode LSR_AbsoluteX   = { 3, 7, EAddressingMode::AbsoluteX   };

///
/// Rotate Left
///

constexpr OpCode ROL_Accumulator = { 1, 2, EAddressingMode::Accumulator };
constexpr OpCode ROL_ZeroPage    = { 2, 5, EAddressingMode::ZeroPage    };
constexpr OpCode ROL_ZeroPageX   = { 2, 6, EAddressingMode::ZeroPageX   };
constexpr OpCode ROL_Absolute    = { 3, 6, EAddressingMode::Absolute    };
constexpr OpCode ROL_AbsoluteX = { 3, 7, EAddressingMode::AbsoluteX };

///
/// Rotate Right
///

constexpr OpCode ROR_Accumulator = { 1, 2, EAddressingMode::Accumulator };
constexpr OpCode ROR_ZeroPage    = { 2, 5, EAddressingMode::ZeroPage    };
constexpr OpCode ROR_ZeroPageX   = { 2, 6, EAddressingMode::ZeroPageX   };
constexpr OpCode ROR_Absolute    = { 3, 6, EAddressingMode::Absolute    };
constexpr OpCode ROR_AbsoluteX   = { 3, 7, EAddressingMode::AbsoluteX   };

///
/// Jumps
/// 

constexpr OpCode JMP_Absolute = { 3, 3, EAddressingMode::Absolute };
constexpr OpCode JMP_Indirect = { 3, 5, EAddressingMode::Indirect };
constexpr OpCode JSR_Absolute = { 3, 6, EAddressingMode::Absolute };
constexpr OpCode RTS_Implied  = { 1, 6, EAddressingMode::Implied  };
                                  
///                               
/// Branches                      
///                               
                                  
constexpr OpCode BCC_Relative = { 2, 2, EAddressingMode::Relative };
constexpr OpCode BCS_Relative = { 2, 2, EAddressingMode::Relative };
constexpr OpCode BEQ_Relative = { 2, 2, EAddressingMode::Relative };
constexpr OpCode BMI_Relative = { 2, 2, EAddressingMode::Relative };
constexpr OpCode BNE_Relative = { 2, 2, EAddressingMode::Relative };
constexpr OpCode BPL_Relative = { 2, 2, EAddressingMode::Relative };
constexpr OpCode BVC_Relative = { 2, 2, EAddressingMode::Relative };
constexpr OpCode BVS_Relative = { 2, 2, EAddressingMode::Relative };

///
/// Clear Status Flags
/// 

constexpr OpCode CLC_Implied = { 1, 2, EAddressingMode::Implied };
constexpr OpCode CLD_Implied = { 1, 2, EAddressingMode::Implied };
constexpr OpCode CLI_Implied = { 1, 2, EAddressingMode::Implied };
constexpr OpCode CLV_Implied = { 1, 2, EAddressingMode::Implied };
                                 
///                              
/// Set Status Flags             
///                              
                                 
constexpr OpCode SEC_Implied = { 1, 2, EAddressingMode::Implied };
constexpr OpCode SED_Implied = { 1, 2, EAddressingMode::Implied };
constexpr OpCode SEI_Implied = { 1, 2, EAddressingMode::Implied };
                                 
///                              
/// System Functions             
///                              
                                 
constexpr OpCode BRK_Implied = { 1, 7, EAddressingMode::Implied };
constexpr OpCode NOP_Implied = { 1, 2, EAddressingMode::Implied };
constexpr OpCode RTI_Implied = { 1, 6, EAddressingMode::Implied };

///
/// Illegal DCP
/// 

constexpr OpCode DCP_ZeroPage  = { 2, 5, EAddressingMode::ZeroPage  };
constexpr OpCode DCP_ZeroPageX = { 2, 6, EAddressingMode::ZeroPageX };
constexpr OpCode DCP_Absolute  = { 3, 6, EAddressingMode::Absolute  };
constexpr OpCode DCP_AbsoluteX = { 3, 7, EAddressingMode::AbsoluteX };
constexpr OpCode DCP_AbsoluteY = { 3, 7, EAddressingMode::AbsoluteY };
constexpr OpCode DCP_IndirectX = { 2, 8, EAddressingMode::IndirectX };
constexpr OpCode DCP_IndirectY = { 2, 8, EAddressingMode::IndirectY };

///
/// Illegal ISB/ISC
/// 

constexpr OpCode ISB_ZeroPage  = { 2, 5, EAddressingMode::ZeroPage  };
constexpr OpCode ISB_ZeroPageX = { 2, 6, EAddressingMode::ZeroPageX };
constexpr OpCode ISB_Absolute  = { 3, 6, EAddressingMode::Absolute  };
constexpr OpCode ISB_AbsoluteX = { 3, 7, EAddressingMode::AbsoluteX };
constexpr OpCode ISB_AbsoluteY = { 3, 7, EAddressingMode::AbsoluteY };
constexpr OpCode ISB_IndirectX = { 2, 8, EAddressingMode::IndirectX };
constexpr OpCode ISB_IndirectY = { 2, 8, EAddressingMode::IndirectY };

///
/// Illegal LAX
///

constexpr OpCode LAX_ZeroPage  = { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode LAX_ZeroPageY = { 2, 4, EAddressingMode::ZeroPageY };
constexpr OpCode LAX_Absolute  = { 3, 4, EAddressingMode::Absolute  };
constexpr OpCode LAX_AbsoluteY = { 3, 4, EAddressingMode::AbsoluteY };
constexpr OpCode LAX_IndirectX = { 2, 6, EAddressingMode::IndirectX };
constexpr OpCode LAX_IndirectY = { 2, 5, EAddressingMode::IndirectY };

///
/// Illegal RLA
/// 

constexpr OpCode RLA_ZeroPage  = { 2, 5, EAddressingMode::ZeroPage  };
constexpr OpCode RLA_ZeroPageX = { 2, 6, EAddressingMode::ZeroPageX };
constexpr OpCode RLA_Absolute  = { 3, 6, EAddressingMode::Absolute  };
constexpr OpCode RLA_AbsoluteX = { 3, 7, EAddressingMode::AbsoluteX };
constexpr OpCode RLA_AbsoluteY = { 3, 7, EAddressingMode::AbsoluteY };
constexpr OpCode RLA_IndirectX = { 2, 8, EAddressingMode::IndirectX };
constexpr OpCode RLA_IndirectY = { 2, 8, EAddressingMode::IndirectY };

///
/// Illegal RRA
/// 

constexpr OpCode RRA_ZeroPage  = { 2, 5, EAddressingMode::ZeroPage  };
constexpr OpCode RRA_ZeroPageX = { 2, 6, EAddressingMode::ZeroPageX };
constexpr OpCode RRA_Absolute  = { 3, 6, EAddressingMode::Absolute  };
constexpr OpCode RRA_AbsoluteX = { 3, 7, EAddressingMode::AbsoluteX };
constexpr OpCode RRA_AbsoluteY = { 3, 7, EAddressingMode::AbsoluteY };
constexpr OpCode RRA_IndirectX = { 2, 8, EAddressingMode::IndirectX };
constexpr OpCode RRA_IndirectY = { 2, 8, EAddressingMode::IndirectY };

///
/// Illegal SAX
/// 

constexpr OpCode SAX_ZeroPage  = { 2, 5, EAddressingMode::ZeroPage  };
constexpr OpCode SAX_ZeroPageY = { 2, 6, EAddressingMode::ZeroPageY };
constexpr OpCode SAX_Absolute  = { 3, 6, EAddressingMode::Absolute  };
constexpr OpCode SAX_IndirectX = { 2, 8, EAddressingMode::IndirectX };

///
/// Illegal SLO
/// 

constexpr OpCode SLO_ZeroPage  = { 2, 5, EAddressingMode::ZeroPage  };
constexpr OpCode SLO_ZeroPageX = { 2, 6, EAddressingMode::ZeroPageX };
constexpr OpCode SLO_Absolute  = { 3, 6, EAddressingMode::Absolute  };
constexpr OpCode SLO_AbsoluteX = { 3, 7, EAddressingMode::AbsoluteX };
constexpr OpCode SLO_AbsoluteY = { 3, 7, EAddressingMode::AbsoluteY };
constexpr OpCode SLO_IndirectX = { 2, 8, EAddressingMode::IndirectX };
constexpr OpCode SLO_IndirectY = { 2, 8, EAddressingMode::IndirectY };

///
/// Illegal SRE
/// 

constexpr OpCode SRE_ZeroPage  = { 2, 5, EAddressingMode::ZeroPage  };
constexpr OpCode SRE_ZeroPageX = { 2, 6, EAddressingMode::ZeroPageX };
constexpr OpCode SRE_Absolute  = { 3, 6, EAddressingMode::Absolute  };
constexpr OpCode SRE_AbsoluteX = { 3, 7, EAddressingMode::AbsoluteX };
constexpr OpCode SRE_AbsoluteY = { 3, 7, EAddressingMode::AbsoluteY };
constexpr OpCode SRE_IndirectX = { 2, 8, EAddressingMode::IndirectX };
constexpr OpCode SRE_IndirectY = { 2, 8, EAddressingMode::IndirectY };

///
/// Illegal NOP
///

constexpr OpCode NOP_Immediate = { 2, 2, EAddressingMode::Immediate };
constexpr OpCode NOP_ZeroPage  = { 2, 3, EAddressingMode::ZeroPage  };
constexpr OpCode NOP_ZeroPageX = { 2, 4, EAddressingMode::ZeroPageX };
constexpr OpCode NOP_Absolute  = { 3, 4, EAddressingMode::Absolute  };
constexpr OpCode NOP_AbsoluteX = { 3, 4, EAddressingMode::AbsoluteX };

///
/// Unimplemented opcodes
///

constexpr OpCode OP_NotImplemented = { 0, 0, EAddressingMode::Implied };