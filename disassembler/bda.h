#ifndef _BDA_H_
#define _BDA_H_

/* Processors */
#define CPU_6502 1
#define CPU_6809 2

/* The 6502's 13 addressing modes */
#define IMMED 0 /* Immediate */
#define ABSOL 1 /* Absolute */     
#define ZEROP 2 /* Zero Page */
#define IMPLI 3 /* Implied */
#define INDIA 4 /* Indirect Absolute */
#define ABSIX 5 /* Absolute indexed with X */
#define ABSIY 6 /* Absolute indexed with Y */
#define ZEPIX 7 /* Zero page indexed with X */
#define ZEPIY 8 /* Zero page indexed with Y */
#define INDIN 9 /* Indexed indirect (with x) */
#define ININD 10 /* Indirect indexed (with y) */
#define RELAT 11 /* Relative */
#define ACCUM 12 /* Accumulator */

/* The following added to support the 6809 */
#define LRELAT 13 /* Long (16-bit) relative */
#define LONG10 14 /* Some 16-bit instructions */
#define LONG11 15 /* Some more 16-bit instructions */
#define DIRECT 16 /* Direct addressing */
#define INDEXED 17 /* Indexed addressing */
#define EXTENDED 18 /* Extended addressing */
#define INHERENT 19 /* Inherent instructions */

#define K64 (64*1024)

/* 6502 vectors */
#define IRQ_VEC_6502 0xfffa
#define RESET_VEC_6502 0xfffc
#define NMI_VEC_6502 0xfffe

/* 6809 vectors */
#define RESET_VEC_6809 0xfffe
#define NMI_VEC_6809 0xfffc
#define SWI_VEC_6809 0xfffa
#define IRQ_VEC_6809 0xfff8
#define FIRQ_VEC_6809 0xfff6
#define SWI2_VEC_6809 0xfff4
#define SWI3_VEC_6809 0xfff2

#define MEM_BYTE 1
#define MEM_CODE 2
#define MEM_WORD 3
#define MEM_EMPTY 4

#define MAXSTR 512
#define MAX_BYTES_PER_LINE 16
#define MAX_WORDS_PER_LINE 8

#define YES 1
#define NO 0

#define CONTINUE_CHAR '\\'
#define EOS '\0'

typedef struct OPcode {
  unsigned char op; /* Number of the opcode */
  unsigned char *name; /* Index in the name table */
  unsigned char addressing; /* Addressing mode */
} OPcode;

typedef struct Label {
  unsigned short address;
  char *name;
  struct Label *next;
} Label;

typedef struct OutputItem {
  char inst[MAXSTR];
  char comment[MAXSTR];
  char label[MAXSTR];
  unsigned short address;
} OutputItem;

#include "proto.h"

#endif
