/* Opcode table */
static const OPcode opcode_table[] = {
  {0x69, "ADC", IMMED}, /* ADC */
  {0x65, "ADC", ZEROP},
  {0x75, "ADC", ZEPIX},
  {0x6D, "ADC", ABSOL},
  {0x7D, "ADC", ABSIX},
  {0x79, "ADC", ABSIY},
  {0x61, "ADC", INDIN},
  {0x71, "ADC", ININD},
 
  {0x29, "AND", IMMED}, /* AND */
  {0x25, "AND", ZEROP},
  {0x35, "AND", ZEPIX},
  {0x2D, "AND", ABSOL},
  {0x3D, "AND", ABSIX},
  {0x39, "AND", ABSIY},
  {0x21, "AND", INDIN},
  {0x31, "AND", ININD},

  {0x0A, "ASL", ACCUM}, /* ASL */
  {0x06, "ASL", ZEROP}, 
  {0x16, "ASL", ZEPIX},
  {0x0E, "ASL", ABSOL},
  {0x1E, "ASL", ABSIX},

  {0x90, "BCC", RELAT}, /* BCC */

  {0xB0, "BCS", RELAT}, /* BCS */

  {0xF0, "BEQ", RELAT}, /* BEQ */

  {0x24, "BIT", ZEROP}, /* BIT */
  {0x2C, "BIT", ABSOL}, 

  {0x30, "BMI", RELAT}, /* BMI */

  {0xD0, "BNE", RELAT}, /* BNE */

  {0x10, "BPL", RELAT}, /* BPL */

  {0x00, "BRK", IMPLI}, /* BRK */

  {0x50, "BVC", RELAT}, /* BVC */ 

  {0x70, "BVS", RELAT}, /* BVS */ 

  {0x18, "CLC", IMPLI}, /* CLC */ 

  {0xD8, "CLD", IMPLI}, /* CLD */ 

  {0x58, "CLI", IMPLI}, /* CLI */ 

  {0xB8, "CLV", IMPLI}, /* CLV */ 
  
  {0xC9, "CMP", IMMED}, /* CMP */
  {0xC5, "CMP", ZEROP},
  {0xD5, "CMP", ZEPIX},
  {0xCD, "CMP", ABSOL},
  {0xDD, "CMP", ABSIX},
  {0xD9, "CMP", ABSIY},
  {0xC1, "CMP", INDIN},
  {0xD1, "CMP", ININD},

  {0xE0, "CPX", IMMED}, /* CPX */
  {0xE4, "CPX", ZEROP},
  {0xEC, "CPX", ABSOL},

  {0xC0, "CPY", IMMED}, /* CPY */
  {0xC4, "CPY", ZEROP},
  {0xCC, "CPY", ABSOL},

  {0xC6, "DEC", ZEROP}, /* DEC */
  {0xD6, "DEC", ZEPIX},
  {0xCE, "DEC", ABSOL},
  {0xDE, "DEC", ABSIX},

  {0xCA, "DEX", IMPLI}, /* DEX */
  
  {0x88, "DEY", IMPLI}, /* DEY */

  {0x49, "EOR", IMMED}, /* EOR */
  {0x45, "EOR", ZEROP},
  {0x55, "EOR", ZEPIX},
  {0x4D, "EOR", ABSOL},
  {0x5D, "EOR", ABSIX},
  {0x59, "EOR", ABSIY},
  {0x41, "EOR", INDIN},
  {0x51, "EOR", ININD},

  {0xE6, "INC", ZEROP}, /* INC */
  {0xF6, "INC", ZEPIX},
  {0xEE, "INC", ABSOL},
  {0xFE, "INC", ABSIX},

  {0xE8, "INX", IMPLI}, /* INX */
  
  {0xC8, "INY", IMPLI}, /* INY */

  {0x4C, "JMP", ABSOL}, /* JMP */
  {0x6C, "JMP", INDIA},

  {0x20, "JSR", ABSOL}, /* JSR */

  {0xA9, "LDA", IMMED}, /* LDA */
  {0xA5, "LDA", ZEROP},
  {0xB5, "LDA", ZEPIX},
  {0xAD, "LDA", ABSOL},
  {0xBD, "LDA", ABSIX},
  {0xB9, "LDA", ABSIY},
  {0xA1, "LDA", INDIN},
  {0xB1, "LDA", ININD},

  {0xA2, "LDX", IMMED}, /* LDX */
  {0xA6, "LDX", ZEROP},
  {0xB6, "LDX", ZEPIY},
  {0xAE, "LDX", ABSOL},
  {0xBE, "LDX", ABSIY},

  {0xA0, "LDY", IMMED}, /* LDY */
  {0xA4, "LDY", ZEROP},
  {0xB4, "LDY", ZEPIX},
  {0xAC, "LDY", ABSOL},
  {0xBC, "LDY", ABSIX},

  {0x4A, "LSR", ACCUM}, /* LSR */
  {0x46, "LSR", ZEROP}, 
  {0x56, "LSR", ZEPIX},
  {0x4E, "LSR", ABSOL},
  {0x5E, "LSR", ABSIX},

  {0xEA, "NOP", IMPLI}, /* NOP */

  {0x09, "ORA", IMMED}, /* ORA */
  {0x05, "ORA", ZEROP},
  {0x15, "ORA", ZEPIX},
  {0x0D, "ORA", ABSOL},
  {0x1D, "ORA", ABSIX},
  {0x19, "ORA", ABSIY},
  {0x01, "ORA", INDIN},
  {0x11, "ORA", ININD},

  {0x48, "PHA", IMPLI}, /* PHA */

  {0x08, "PHP", IMPLI}, /* PHP */

  {0x68, "PLA", IMPLI}, /* PLA */

  {0x28, "PLP", IMPLI}, /* PLP */

  {0x2A, "ROL", ACCUM}, /* ROL */
  {0x26, "ROL", ZEROP}, 
  {0x36, "ROL", ZEPIX},
  {0x2E, "ROL", ABSOL},
  {0x3E, "ROL", ABSIX},

  {0x6A, "ROR", ACCUM}, /* ROR */
  {0x66, "ROR", ZEROP}, 
  {0x76, "ROR", ZEPIX},
  {0x6E, "ROR", ABSOL},
  {0x7E, "ROR", ABSIX},

  {0x40, "RTI", IMPLI}, /* RTI */

  {0x60, "RTS", IMPLI}, /* RTS */

  {0xE9, "SBC", IMMED}, /* SBC */
  {0xE5, "SBC", ZEROP},
  {0xF5, "SBC", ZEPIX},
  {0xED, "SBC", ABSOL},
  {0xFD, "SBC", ABSIX},
  {0xF9, "SBC", ABSIY},
  {0xE1, "SBC", INDIN},
  {0xF1, "SBC", ININD},

  {0x38, "SEC", IMPLI}, /* SEC */
 
  {0xF8, "SED", IMPLI}, /* SED */

  {0x78, "SEI", IMPLI}, /* SEI */

  {0x85, "STA", ZEROP}, /* STA */
  {0x95, "STA", ZEPIX},
  {0x8D, "STA", ABSOL},
  {0x9D, "STA", ABSIX},
  {0x99, "STA", ABSIY},
  {0x81, "STA", INDIN},
  {0x91, "STA", ININD},

  {0x86, "STX", ZEROP}, /* STX */
  {0x96, "STX", ZEPIY},
  {0x8E, "STX", ABSOL},

  {0x84, "STY", ZEROP}, /* STY */
  {0x94, "STY", ZEPIX},
  {0x8C, "STY", ABSOL},

  {0xAA, "TAX", IMPLI}, /* TAX */

  {0xA8, "TAY", IMPLI}, /* TAY */
 
  {0xBA, "TSX", IMPLI}, /* TSX */
 
  {0x8A, "TXA", IMPLI}, /* TXA */
 
  {0x9A, "TXS", IMPLI}, /* TXS */

  {0x98, "TYA", IMPLI} /* TYA */
};
