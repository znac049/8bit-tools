#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conv.h"

void IntelHex::readIn(FILE *fd) {
  char line[MAXSTR];
  int res;
  
  while (fgets(line, MAXSTR, fd) != NULL) {
    Utils::trim(line);

    res = process(line);
  }
}

void IntelHex::writeOut(FILE *fd) {
  const int maxBytesPerLine = 16;
  long lowest = getStartAddress();
  long highest = getEndAddress();
  int xSum;

  long baseAddress = lowest;

  if (relocateTo != -1) {
    baseAddress = relocateTo;
  }

  for(long addr = lowest; addr < highest; addr += maxBytesPerLine, baseAddress += maxBytesPerLine) {
    int count = maxBytesPerLine;

    if (addr + count > highest) {
      count = highest - addr + 1;
    }

    xSum =  count & 0xff;
    //printf("xSum  %02X\n", xSum);

    xSum += ((baseAddress >> 8) & 0xff);
    //printf("xSum  %02X\n", xSum);

    xSum += (baseAddress & 0xff);
    //printf("xSum  %02X\n", xSum);

    fprintf(fd, ":%02X%04X00", count, baseAddress);

    for (long i=0; i<count; i++) {
      int val = mem->getByte(addr+i);

      xSum += (val & 0xff);

      //printf("xSum  %02X\n", xSum);

      fprintf(fd, "%02X", val & 0xff);
    }

    xSum = (~xSum + 1) & 0xff;
    //printf("xSum  %02X\n\n", xSum);
    
    fprintf(fd, "%02X\n", xSum);
  }

  fprintf(fd, ":00000001FF\n");
}

int IntelHex::convert(const char *str, int nChars) {
  int res = 0;
  int nibble;
  int i;

  for (i=0; i<nChars; i++) {
    nibble = Utils::hexVal(str[i]);

    if (nibble == -1) {
      return -1;
    }

    res = (res << 4) | nibble;
  }

  return res;
}

int IntelHex::process(const char *line) {
  int len = strlen(line);

  return IHEX_OK;
}

