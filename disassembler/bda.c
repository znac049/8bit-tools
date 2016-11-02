/**************************************************************/
/* DCC6502.c -> Main module of:                               */
/* Disassembler and Cycle Counter for the 6502 microprocessor */
/*                                                            */
/* (C) 1998-2001 Tennessee Carmel-Veilleux(veilleux@ameth.org)*/
/* This code is offered as FREEware. You cannot modify nor    */
/* distribute modified versions of this software without      */
/* prior written consent of the author(s). The author shall   */
/* NOT be responsible for ANY damage purely physical,         */
/* emotional, material and magical to either you or anyone    */
/* in the universe (not excluding GOD). That means: enjoy it  */
/* and perhaps kill yourself with this, I don't care, ain't   */
/* my problem neither.                                        */
/**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <getopt.h>

#include "bda.h"

char hex_output = NO; /* 1 if hex output is desired at beginning of line */
int verbose = NO;

int cpu = CPU_NONE;

char src_file[MAXSTR];
char label_file[MAXSTR];

unsigned char byteStack[MAX_BYTES_PER_LINE];
int byteSP = 0;
int byteAddr = 0;

unsigned short wordStack[MAX_WORDS_PER_LINE];
int wordSP = 0;
int wordAddr = 0;

int (*disassemble)(int, OutputItem*);

void usage_helper(char *str) {
  fprintf(stderr,"\t%s\n",str);
}

void usage(void) {
  usage_helper("-?      -> Show this help message");
  usage_helper("-v      -> Produce verbose output during disassembly");

  fprintf(stderr,"\n");
}

void parse_args(int argc, char *argv[]) {
  int opt = 0;
  int long_index = 0;

  static struct option long_options[] = {
    {"output-hex",  no_argument,       NULL,  'h' },
    {"label-file",  required_argument, NULL,  'l' },
    {"machine",     required_argument, NULL,  'm' },
    {"help",        no_argument,       NULL,  '?' },
    {"verbose",     no_argument,       NULL,  'v' },
    {NULL,          0,                 NULL,  0   }
  };

  strcpy(src_file, "");
  strcpy(label_file, "");

  while ((opt = getopt_long(argc, argv,"hi:n:r:b:s:v", long_options, &long_index )) != -1) {
    switch (opt) {
    case 'h':
      hex_output = YES;
      break;

    case 'l':
      strcpy(label_file, optarg);
      break;

    case 'm':
      read_machine_file(optarg);
      break;

    case 'v':
      verbose = YES;
      break;

    case '?':
    default: 
      usage(); 
      exit(1);
    }
  }

  dump_cfg();

  /* There should be just one arg left, the name of the ROM file */
  if ((optind+1) != argc) {
    fprintf(stderr, "ROM file name missing.\n");
    usage();
    exit(1);
  }

  if (label_file[0]) {
    /* Make sure the label file specified actually exists */
    if (!is_file(label_file)) {
      fprintf(stderr, "Label file '%s'not found or is not a regular file.\n", label_file);
      exit(1);
    }
  }

  strncpy(src_file, argv[argc-1], MAXSTR-1);
}

void clear_output_item(OutputItem *oi) {
  strcpy(oi->label, "");
  strcpy(oi->inst, "");
  strcpy(oi->comment, "");

  oi->address = 0;
}

void select_cpu(char *str) {
  if (strcasecmp(str, "6502") == 0) {
    cpu = CPU_6502;
  }
  else if (strcasecmp(str, "6809") == 0) {
    cpu = CPU_6809;
  }
  else {
    printf("Unknown CPU type specified\n");
    usage();
    exit(1);
  }
}

void set_starting_address(int vector, char *lab) {
  unsigned short addr;

  if (get_memory_type(vector) != MEM_EMPTY) {
    char vecLab[MAXSTR];

    addr = get_word(vector);

    snprintf(vecLab, MAXSTR-1, "%sVector", lab);
    create_label(vecLab, vector);
  }

  if (addr != 0xffff) {
    stack_address(addr);
    create_label(lab, addr);
  }
}

void set_starting_point() {
  switch (cpu) {
  case CPU_6502:
    set_starting_address(RESET_VEC_6502, "Reset");
    set_starting_address(IRQ_VEC_6502, "IRQ");
    set_starting_address(NMI_VEC_6502, "NMI");
    break;

  case CPU_6809:
    set_starting_address(RESET_VEC_6809, "Reset");
    set_starting_address(NMI_VEC_6809, "NMI");
    set_starting_address(SWI_VEC_6809, "SWI");
    set_starting_address(IRQ_VEC_6809, "IRQ");
    set_starting_address(FIRQ_VEC_6809, "FIRQ");
    set_starting_address(SWI2_VEC_6809, "SWI2");
    set_starting_address(SWI3_VEC_6809, "SWI3");
    break;
  }
}

void pass1() {
  int addr;
  int nBytes;
  OutputItem inst;
  int i;

  set_starting_point();

  switch (cpu) {
  case CPU_6502:
    disassemble = disassemble_6502;
    break;

  case CPU_6809:
    disassemble = disassemble_6809;
    break;
  }

  if (verbose) {
    printf("Pass 1.\n");
  }

  for (i = 0; i < get_address_stack_size(); i++) {
    addr = get_address_stack_at(i);

    if (verbose) {
      printf("Disassemble from address $%04x\n", addr);
    }

    /* Disassemble each chunk until we hit an unknown instruction */
    nBytes = disassemble(addr, &inst);
    while (nBytes > 0) {
      addr += nBytes;
      nBytes = disassemble(addr, &inst);
    }
  }
}

void push_byte(unsigned char b, int addr) {
  if ( (byteSP >= MAX_BYTES_PER_LINE) || 
       (byteSP && ((addr % MAX_BYTES_PER_LINE) == 0)) ||
       (is_label(addr) && byteSP) ) {
    emit_bytes(&byteAddr, byteStack, &byteSP);
    byteAddr = addr;
  }

  if (byteAddr == -1) {
    byteAddr = addr;
  }

  byteStack[byteSP++] = b;
}

void push_word(unsigned short w, int addr) {
  if ( (wordSP >= MAX_WORDS_PER_LINE) || 
       (wordSP && ((addr % MAX_WORDS_PER_LINE) == 0)) ||
       (is_label(addr) && wordSP) ) {
    emit_words(&wordAddr, wordStack, &wordSP);
    wordAddr = addr;
  }

  if (wordAddr == -1) {
    wordAddr = addr;
  }

  wordStack[byteSP++] = w;
}

void emit_bytes(int *addr, unsigned char *stack, int *sp) {
  char lab[MAXSTR];
  int i;
  int numBytes = *sp;

  if (is_label(*addr)) {
    lookup_label(*addr, lab, MAXSTR-1);
  }
  else {
    lab[0] = EOS;
  }

  printf("%-23s %-20s ", lab, "!byte");
  for (i=0; i<numBytes; i++) {
    printf("$%02x", stack[i]);

    if ((i+1) < numBytes) {
      putchar(',');
    }
  }

  for (i=numBytes; i<MAX_BYTES_PER_LINE; i++) {
    printf("    ");
  }

  printf(" ; %04x\n", *addr);

  *addr = -1;
  *sp = 0;
}

void emit_words(int *addr, unsigned short *stack, int *sp) {
  char lab[MAXSTR];
  int i;
  int numWords = *sp;

  if (is_label(*addr)) {
    lookup_label(*addr, lab, MAXSTR-1);
  }
  else {
    lab[0] = EOS;
  }

  printf("%-23s %-20s ", lab, "!word");
  for (i=0; i<numWords; i++) {
    printf("$%04x", stack[i]);

    if ((i+1) < numWords) {
      putchar(',');
    }
  }

  for (i=numWords; i<MAX_BYTES_PER_LINE; i++) {
    printf("      ");
  }

  printf(" ; %04x\n", *addr);

  *addr = -1;
  *sp = 0;
}

void generate_listing() {
  OutputItem inst;
  int addr;
  int nBytes;
  int prevType = MEM_EMPTY;
  int org_needed = YES;

  if (verbose) {
    dump_memory_info();
  }

  byteAddr = wordAddr = 0;

  for (addr = 0; addr < K64;) {
    int memType = get_memory_type(addr);

    /* We may have previous items (bytes/words) that haven't been emitted yet */
    if (prevType != memType) {
      switch (prevType) {
      case MEM_BYTE:
	emit_bytes(&byteAddr, byteStack, &byteSP);
	break;

      case MEM_WORD:
	emit_words(&wordAddr, wordStack, &wordSP);
	break;
      }

      printf("\n");
    }

    switch (memType) {
    case MEM_BYTE:
      if (org_needed == YES) {
	printf("                 ORG $%04x\n", addr);
	org_needed = NO;
      }

      push_byte(get_byte(addr), addr);
      nBytes = 1;
      break;

    case MEM_CODE:
      if (org_needed == YES) {
	printf("                 ORG $%04x\n", addr);
	org_needed = NO;
      }

      if (is_entry_point(addr)) {
	printf("\n");
      }

      nBytes = disassemble(addr, &inst);

      printf("%-23s %-20s ; %04X %s\n", inst.label, inst.inst, inst.address, inst.comment);
      break;

    case MEM_WORD:
      if (org_needed == YES) {
	printf("                 ORG $%04x\n", addr);
	org_needed = NO;
      }

      push_word(get_word(addr), addr);
      nBytes = 2;
      break;

    case MEM_EMPTY:
      nBytes = 1;
      org_needed = YES;
      break;
    }

    if (nBytes < 0) {
      nBytes = -nBytes;
    }

    if (is_label(addr)) {
      lookup_label(addr, inst.label, MAXSTR-1);
    }

    addr += nBytes;

    prevType = memType;
  }

  if (byteSP) {
    emit_bytes(&byteAddr, byteStack, &byteSP);
  }
  else if (wordSP) {
    emit_words(&wordAddr, wordStack, &wordSP);
  }

  dump_labels();
}

int main(int argc, char *argv[]) {
  init_memory();

  parse_args(argc, argv);
   
  /* if (read_file(src_file, prog_start) == NO) {
    fprintf(stderr, "Problem reading source file '%s'\n", src_file);
    exit(1);
    } */

  read_label_file(label_file);

  pass1();
  generate_listing();

  return 0;
}
