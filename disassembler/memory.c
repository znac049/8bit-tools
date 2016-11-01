#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "bda.h"

unsigned char Memory[K64];
unsigned char MemoryType[K64];

void init_memory() {
  memset(Memory, 0, K64);
  memset(MemoryType, MEM_EMPTY, K64);
  memory_is_word(0xfffa, 3);
}

void dump_memory_info() {
  int addr;

  for (addr=0; addr<K64; addr+= 32) {
    int j;
    int empty = YES;

    for (j=0; j<32; j++) {
      if (MemoryType[addr+j] != MEM_EMPTY) {
	empty = NO;
      }
    }

    if (!empty) {
      printf("%04x: ", addr);
      for (j=0; j<32; j++) {
	char c = '?';

	switch (MemoryType[addr+j]) {
	case MEM_BYTE:
	  c = 'b';
	  break;

	case MEM_WORD:
	  c = 'w';
	  break;

	case MEM_CODE:
	  c = 'C';
	  break;
	}

	putchar(c);
      }

      putchar('\n');
    }
  }
}

int read_file(const char *file_name, int addr) {
  FILE *fd = fopen(file_name, "rb");
  int nBytes = 0;

  if (!fd) {
    return NO;
  }

  while(!feof(fd)) {
    Memory[addr+nBytes] = fgetc(fd);
    MemoryType[addr+nBytes] = MEM_BYTE;
    nBytes++;
  }

  fclose(fd);

  return nBytes;
}

unsigned char get_byte(int addr) {
  return Memory[addr];
}

void set_byte(int addr, unsigned char b) {
  Memory[addr] = b;
}

unsigned short get_word(int addr) {
  unsigned short lsb, msb;

  lsb = Memory[addr];
  msb = Memory[addr+1];

  return lsb | (msb << 8);
}

int memory_is_word(int addr, int nWords) {
  int i;
  int a = addr;

  for (i=0; i<nWords; i++) {
    MemoryType[a++] = MEM_WORD;
    MemoryType[a++] = MEM_WORD;
  }

  return nWords;
}

int memory_is_code(int addr, int nBytes) {
  int i;

  for (i=0; i<nBytes; i++) {
    MemoryType[addr+i] = MEM_CODE;
  }

  return nBytes;
}

unsigned char get_memory_type(int addr) {
  return MemoryType[addr];
}
