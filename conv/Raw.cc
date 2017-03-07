#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conv.h"

void Raw::readIn(FILE *fd) {
  int c;
  long addr=0;
  
  while ((c = fgetc(fd)) != EOF) {
    mem->setByte(addr++, c);
  }
}

void Raw::writeOut(FILE *fd) {
  long lowest = getStartAddress();
  long highest = getEndAddress();

  if (relocateTo != -1) {
    Utils::abortf("--relocate-to has no meaning for a binary file");
  }

  for (long i = lowest; i <= highest; i++) {
    fputc(mem->getByte(i), fd);
  }
}

