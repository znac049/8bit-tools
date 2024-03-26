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

int IntelHex::checksum(const char *line) {
  int count;
  int addr;
  int recordType;

  int xSum;
  int i;

  if (line[0] != ':') {
    return -1;
  }

  count = convert(&line[1], 2);
  addr = convert(&line[3], 4);
  recordType = convert(&line[7], 2);

  xSum = count + ((addr>>8) & 0xff) + (addr & 0xff) + recordType;
  
  for (i=0; i<count; i++) {
    int b = convert(&line[(i*2)+9], 2);

    printf("0x%02x ", b);
    
    xSum += b;
  }
  printf("\n");
  
  return (~xSum + 1) & 0xff;
}

int IntelHex::process(const char *line) {
  int len = strlen(line);

  int count;
  int addr;
  int recordType;

  int xSum;

  int i;

  if (line[0] != ':') {
    return IHEX_BAD;
  }

  if ((len > 0) && (line[len-1] == '\n')) {
    len--;
  }
  
  if ((len > 0) && (line[len-1] == '\r')) {
    len--;
  }
  
  count = convert(&line[1], 2);
  addr = convert(&line[3], 4);
  recordType = convert(&line[7], 2);
  xSum = convert(&line[len-2], 2);

  printf("Line: '%s'\n", line);
  printf("count=%d, addr=0x%04x, type=%d, xsum=0x%02x (%c%c)\n", count, addr, recordType, xSum, line[len-2], line[len-1]);
  printf("calulated xsum=0x%02x\n", checksum(line));

  switch (recordType) {
  case 0:
    for (i=0; i<count; i++) {
      mem->setByte(addr++, convert(&line[(i*2)+9], 2));
    }
    break;

  case 1:
    return IHEX_OK;

  case 2: case 3:
  case 4: case 5:
    return IHEX_UNIMPLEMENTED;

  default:
    return IHEX_UNKNOWNTYPE;
  }
  
  return IHEX_OK;
}

