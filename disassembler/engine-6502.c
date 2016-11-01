#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "bda.h"

#include "inst-6502.inc"

extern int verbose;

static const OPcode *lookup_instruction(unsigned char opcode) {
  int numOpcodes = (sizeof(opcode_table) / sizeof(OPcode)) + 1;
  int i;

  for (i=0; i<numOpcodes; i++) {
    const OPcode *op = &(opcode_table[i]);
 
    if (op->op == opcode) {
      return op;
    }
  }

  if (verbose) {
    printf("Unknown instruction: %02x\n", opcode);
  }

  return NULL;
}

static void dump_opcode(const OPcode *op) {
  printf("    number: %02x\n", op->op);
  printf("      name: %s\n", op->name);
  printf("addressing: %02x\n", op->addressing);
}

/* This function disassembles the opcode at the PC and outputs it in *output */
int disassemble_6502(int addr, OutputItem *res) { 
  unsigned char opcode = get_byte(addr);
  const OPcode *op = lookup_instruction(opcode);
  char argument_signed;
  char inst[MAXSTR];
  int i;
  int j;
  int nBytes = 1;
  char lab[MAXSTR];
  char *mnem;
  unsigned short target;

  if (verbose) {
    printf("Disassemble, addr=%04x, op=%02x\n", addr, opcode);

    if (op) {
      dump_opcode(op);
    }
    else {
      printf("instruction lookup failed\n");
    }
  }

  clear_output_item(res);
  res->address = addr;

  if (is_label(addr)) {
    lookup_label(addr, res->label, MAXSTR-1);
  }

  if (op == NULL) {
    snprintf(res->inst, MAXSTR-1, "!byte $%02x", opcode);

    return -memory_is_code(addr, nBytes);
  }

  switch (op->addressing) {
  case IMMED: 
    nBytes = memory_is_code(addr, 2);
    target = get_byte(addr+1);
    //lookup_label(target_addr, lab, MAXSTR-1);
    snprintf(res->inst, MAXSTR-1, "%s #$%02x", op->name, target);
    break;

  case ABSOL:
    nBytes = memory_is_code(addr, 3);
    target = get_word(addr+1);
    lookup_label(target, lab, MAXSTR-1);

    /* Kludge - JSR and JMP */
    if ((strcasecmp(op->name, "jmp") == 0) || (strcasecmp(op->name, "jsr") == 0)) {
      stack_address(target);
      create_label(NULL, target);
    }

    if (strcasecmp(op->name, "jmp") == 0) {
      nBytes = -nBytes;
    }

    snprintf(res->inst, MAXSTR-1, "%s %s", op->name, lab);
    break;

  case ZEROP:
    nBytes = memory_is_code(addr, 2);
    target = get_byte(addr+1);
    if (is_label(target)) {
      lookup_label(target, lab, MAXSTR-1);
      snprintf(res->inst, MAXSTR-1, "%s %s", op->name, lab);
    }
    else {
      snprintf(res->inst, MAXSTR-1, "%s $%02x", op->name, target);
    }
    break;

  case IMPLI:
    mnem = op->name;
    nBytes = memory_is_code(addr, 1);

    /* Kludge - RTI and RTS */
    if ((strcasecmp(op->name, "rts") == 0) || (strcasecmp(op->name, "rti") == 0)) {
      nBytes = -nBytes;
    }

    snprintf(res->inst, MAXSTR-1, "%s", mnem);
    break;

  case INDIA:
    nBytes = memory_is_code(addr, 3);
    target = get_word(addr+1);
    if (is_label(target)) {
      lookup_label(target, lab, MAXSTR-1);
      snprintf(res->inst, MAXSTR-1, "%s (%s)", op->name, lab);
    }
    else {
      snprintf(res->inst, MAXSTR-1, "%s ($%04x)", op->name, target);
    }
    break;

  case ABSIX:
    nBytes = memory_is_code(addr, 3);
    target = get_word(addr+1);
    if (is_label(target)) {
      lookup_label(target, lab, MAXSTR-1);
      snprintf(res->inst, MAXSTR-1, "%s %s,X", op->name, lab);
    }
    else {
      snprintf(res->inst, MAXSTR-1, "%s $%04x,X", op->name, target);
    }
    break;

  case ABSIY:
    nBytes = memory_is_code(addr, 3);
    target = get_word(addr+1);
    if (is_label(target)) {
      lookup_label(target, lab, MAXSTR-1);
      snprintf(res->inst, MAXSTR-1, "%s %s,Y", op->name, lab);
    }
    else {
      snprintf(res->inst, MAXSTR-1, "%s $%04x,Y", op->name, target);
    }
    break;

  case ZEPIX:
    nBytes = memory_is_code(addr, 2);
    target = get_byte(addr+1);
    if (is_label(target)) {
      lookup_label(target, lab, MAXSTR-1);
      snprintf(res->inst, MAXSTR-1, "%s %s,X", op->name, lab);
    }
    else {
      snprintf(res->inst, MAXSTR-1, "%s $%02x,X", op->name, target);
    }
    break;

  case ZEPIY:
    nBytes = memory_is_code(addr, 2);
    target = get_byte(addr+1);
    if (is_label(target)) {
      lookup_label(target, lab, MAXSTR-1);
      snprintf(res->inst, MAXSTR-1, "%s %s,X", op->name, lab);
    }
    else {
      snprintf(res->inst, MAXSTR-1, "%s $%02x,Y", op->name, target);
    }
    break;

  case INDIN:
    nBytes = memory_is_code(addr, 2);
    target = get_byte(addr+1);
    if (is_label(target)) {
      lookup_label(target, lab, MAXSTR-1);
      snprintf(res->inst, MAXSTR-1, "%s (%s,X)", op->name, lab);
    }
    else {
      snprintf(res->inst, MAXSTR-1, "%s ($%02x,X)", op->name, target);
    }
    break;

  case ININD:
    nBytes = memory_is_code(addr, 2);
    target = get_byte(addr+1);
    if (is_label(target)) {
      lookup_label(target, lab, MAXSTR-1);
      snprintf(res->inst, MAXSTR-1, "%s (%s),Y", op->name, lab);
    }
    else {
      snprintf(res->inst, MAXSTR-1, "%s ($%02x),Y", op->name, target);
    }
    break;

  case RELAT:
    nBytes = memory_is_code(addr, 2);
    target = addr + (signed char)(get_byte(addr+1)) + 2;
    lookup_label(target, lab, MAXSTR-1);
    snprintf(res->inst, MAXSTR-1, "%s %s", op->name, lab);
    stack_address(target);
    create_label(NULL, target);
    break;

  case ACCUM:
    nBytes = memory_is_code(addr, 1);    
    snprintf(res->inst, MAXSTR-1, "%s", op->name);
    break;

  default:
    printf("OOPS!\n");
    nBytes=2000;
    break;
  }

  return nBytes;
}

