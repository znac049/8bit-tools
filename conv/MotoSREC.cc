#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conv.h"

void MotoSREC::readIn(FILE *fd) {
  char line[MAXSTR];
  int res;
  
  while (fgets(line, MAXSTR, fd) != NULL) {
    Utils::trim(line);

    res = process(line);
  }
}

void MotoSREC::writeOut(FILE *fd) {
}

int MotoSREC::convert(const char *str, int nChars) {
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

int MotoSREC::checksum(const char *line) {
  int len = strlen(line);
  int calcSum = 0;
  int count;
  int i;
  const char *payload = &line[2];

  if ((len & ~1) != len) {
    //printf("\n%s\nLength %d is not supposed to be odd\n", line, len);
    return -1;
  }

  count = convert(payload, 2) - 1;
  if (count < 0) {
    //printf("\n%s\nCount should not be negative\n", line);
    return -1;
  }

  for (i=0; i<=count; i++) {
    int bv = convert(payload, 2);

    payload += 2;
    calcSum += bv;
  }

  calcSum = (~calcSum & 0xff);

  return calcSum & 0xff;
}

int MotoSREC::process(const char *line) {
  int len = strlen(line);

  int count;
  long addr;

  int calcSum = 0;
  int xSum;

  const char *bytes;

  // S Records are all at least 10 bytes long
  if (len < 10) {
    return SREC_TOOSHORT;
  }

  // Must start with 'S'
  if (line[0] != 'S') {
    return SREC_BADFORMAT;
  }

  count = convert(&line[2], 2);
  addr = convert(&line[4], 4);

  xSum = convert(&line[2+count+count], 2);
  calcSum = checksum(line);

  if (calcSum != xSum) {
    return SREC_CHECKSUM;
  }

  switch (line[1]) {
  case '0':
    // vendor specific header - text - ignore
    break;

  case '1':
    if ((count < 3) || (addr == -1)) {
      return SREC_BADFORMAT;
    }

    count = (count-3)*2;

    bytes = &line[8];
    for (int i=0; i<count; i++) {
      if (!Utils::isHexChar(bytes[i])) {
	return SREC_BADFORMAT;
      }
    }

    for (int i=0; i<count; i+=2) {
      mem->setByte(addr++, convert(&bytes[i], 2));
    }
    break;

  case '5':
    if ((count != 3) || (addr == -1)) {
      return SREC_BADFORMAT;
    }
    break;

  case '9':
    if ((count != 3) || (addr == -1)) {
      return SREC_BADFORMAT;
    }
    break;
    
  default:
    return SREC_UNIMPLEMENTED;
  }
  
  return SREC_OK;
}

