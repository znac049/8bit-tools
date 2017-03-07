#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "conv.h"

Memory::Memory(long mSize) {
  memorySize = mSize;
  memory.clear();
  memory.reserve(memorySize);

  for (int i=0; i<memorySize; i++) {
    memory[i] = new MemoryCell();
  }

  setType(0, UNKNOWN, memorySize);
  
  padChar = 0xff;

  highestAddress = 0;
  lowestAddress = memorySize-1;
}

long Memory::getLowestAddress() {
  return lowestAddress;
}

long Memory::getHighestAddress() {
  return highestAddress;
}

bool Memory::isValidAddress(long addr) {
  return ((addr >= 0) && (addr < memorySize));
}

void Memory::assertAddressValid(long addr) {
  if (!isValidAddress(addr)) {
    throw RangeException();
  }
}

long Memory::readFile(const char *fileName, long addr) {
  FILE *fd = fopen(fileName, "rb");
  int nBytes = 0;

  printf("Read file '%s' into memory at $%04x\n", fileName, addr);

  if (!fd) {
    return NO;
  }

  while(!feof(fd)) {
    int c = fgetc(fd);

    if (c != EOF) {
      if (isValidAddress(addr)) {
	memory[addr++]->set(c);
      }

      nBytes++;
    }
  }

  fclose(fd);

  printf("%d bytes read\n", nBytes);

  return nBytes;
}

int Memory::getByte(long addr) {
  int res;
  assertAddressValid(addr);

  if (memory[addr]->getType() == UNKNOWN) {
    res = padChar;
  }
  else {
    res = memory[addr]->get();
  }

  return res;
}

void Memory::setByte(long addr, int b) {
  assertAddressValid(addr);

  if (addr > highestAddress) {
    highestAddress = addr;
  }

  if (addr < lowestAddress) {
    lowestAddress = addr;
  }

  memory[addr]->set(b);
  memory[addr]->setType(BYTE);
}

int Memory::setType(long addr, int type, int count) {
  int size = 1;

  count = count * size;
  for (int i=0; i<count; i++) {
    if (isValidAddress(addr+i)) {
      memory[addr+i]->setType(type);
    }
  }

  return count;
}

void Memory::setPadChar(char pad) {
  padChar = pad;
}

void Memory::info() {
  bool empty = true;

  printf("Memory information:\n");
  printf("  Lowest address loaded = $%04x\n", lowestAddress);
  printf("  Highest address loaded = $%04x\n", highestAddress);

  printf("\n  Chunks:\n");
  for (long addr = lowestAddress; addr <= highestAddress; addr++) {
    if (empty && (memory[addr]->getType() != UNKNOWN)) {
      empty = false;
      printf("    $%04x - ", addr);
    }

    if (!empty && (memory[addr]->getType() == UNKNOWN)) {
      empty = true;
      printf("$%04x\n", addr);
    }
  }

  if (!empty) {
    printf("$%04x\n", highestAddress);
  }

  printf("\n");
}

void Memory::dump() {
  int bytesPerLine = 32;
  bool empty;

  for (long addr=0; addr<memorySize; addr+=bytesPerLine) {
    empty = true;

    for (int i=0; i<bytesPerLine; i++) {
      if (memory[addr+i]->getType() != UNKNOWN) {
	empty = false;
      }
    }

    if (!empty) {
      printf("%08x: ", addr);

      for (int i=0; i<bytesPerLine; i++) {
	printf(" %02x", getByte(addr+i));
      }

      printf("\n");
    }
  }
}
