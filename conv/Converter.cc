#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conv.h"

Converter::Converter() {
  mem = NULL;
  relocateTo = -1;
  startAddress = -1;
  endAddress = -1;
}

void Converter::setName(const char *newName) {
  strncpy(name, newName, MAXSTR-1);
}

const char *Converter::getName() {
  return name;
}

void Converter::setMemory(Memory *memory) {
  mem = memory;
}

void Converter::setRelocateTo(long baseAddr) {
  if (verbose) {
    printf("relocate address=$%04x\n", baseAddr);
  }

  relocateTo = baseAddr;
}

void Converter::setStartAddress(long addr) {
  if (verbose) {
    printf("start address=$%04x\n", addr);
  }

  startAddress = addr;
}

void Converter::setEndAddress(long addr) {
  if (verbose) {
    printf("end address=$%04x\n", addr);
  }

  endAddress = addr;
}

long Converter::getStartAddress() {
  long addr = mem->getLowestAddress();

  if (startAddress != -1) {
    addr = startAddress;
  }

  return addr;
}

long Converter::getEndAddress() {
  long addr = mem->getLowestAddress();

  if (endAddress != -1) {
    addr = endAddress;
  }

  return addr;
}

void Converter::setVerbose(bool v) {
  verbose = v;
}
