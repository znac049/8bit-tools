static OPcode opcode_table [] = {
  {0x3a, "ABX", INHERENT}, /* ABX */

  {0x89, "ADCA", IMMED}, /* ADCA */
  {0x99, "ADCA", DIRECT}, 
  {0xa9, "ADCA", INDEXED},
  {0xb9, "ADCA", EXTENDED},

  {0xc9, "ADCB", IMMED}, /* ADCB */
  {0xd9, "ADCB", DIRECT}, 
  {0xe9, "ADCB", INDEXED},
  {0xf9, "ADCB", EXTENDED},

  {0x88, "ADDA", IMMED}, /* ADDA */
  {0x98, "ADDA", DIRECT}, 
  {0xa8, "ADDA", INDEXED},
  {0xb8, "ADDA", EXTENDED},

  {0xcb, "ADDB", IMMED}, /* ADDB */
  {0xdb, "ADDB", DIRECT}, 
  {0xeb, "ADDB", INDEXED},
  {0xfb, "ADDB", EXTENDED},

  {0xc3, "ADDD", IMMED}, /* ADDD */
  {0xd3, "ADDD", DIRECT}, 
  {0xe3, "ADDD", INDEXED},
  {0xf3, "ADDD", EXTENDED},

  {0x24, "BCC", RELAT}, /* BCC */

  {0x25, "BCS", RELAT}, /* BCS */

  {0x27, "BEQ", RELAT}, /* BEQ */

  {0x2c, "BGE", RELAT}, /* BGE */

  {0x2e, "BGT", RELAT}, /* BGT */

  {0x22, "BMI", RELAT}, /* BMI */

  {0x24, "BHS", RELAT}, /* BHS */

  {0x2f, "BLE", RELAT}, /* BLE */

  {0x25, "BLO", RELAT}, /* BLO */

  {0x23, "BLS", RELAT}, /* BLS */

  {0x2d, "BLT", RELAT}, /* BLT */

  {0x2b, "BMI", RELAT}, /* BMI */

  {0x26, "BNE", RELAT}, /* BNE */

  {0x2a, "BPL", RELAT}, /* BPL */

  {0x20, "BRA", RELAT}, /* BRA */

  {0x21, "BRN", RELAT}, /* BRN */

  {0x8d, "BSR", RELAT}, /* BSR */

  {0x28, "BVC", RELAT}, /* BVC */

  {0x29, "BVS", RELAT}, /* BVS */

  {0x16, "LBRA", LRELAT}, /* LBRA */

  {0x17, "LBSR", LRELAT}, /* LBSR */
};

OPcode opcode_table_10 [] = {
  {0x24, "LBCC", LRELAT}, /* LBCC */

  {0x25, "LBCS", LRELAT}, /* LBCS */

  {0x27, "LBEQ", LRELAT}, /* LBEQ */

  {0x2c, "LBGE", LRELAT}, /* LBGE */

  {0x2e, "LBGT", LRELAT}, /* LBGE */

  {0x22, "LBMI", LRELAT}, /* LBGE */

  {0x24, "LBHS", LRELAT}, /* LBGE */

  {0x2f, "LBLE", LRELAT}, /* LBGE */

  {0x25, "LBLO", LRELAT}, /* LBGE */

  {0x23, "LBLS", LRELAT}, /* LBGE */

  {0x2d, "LBLT", LRELAT}, /* LBGE */

  {0x2B, "LBMI", LRELAT}, /* LBGE */

  {0x26, "LBNE", LRELAT}, /* LBGE */

  {0x2a, "LBPL", LRELAT}, /* LBGE */

  {0x21, "LBRN", LRELAT}, /* LBGE */

  {0x28, "LBVC", LRELAT}, /* LBGE */

  {0x29, "LBVC", LRELAT}, /* LBGE */
};

OPcode opcode_table_11 [] = {
};
