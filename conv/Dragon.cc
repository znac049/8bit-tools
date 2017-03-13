#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conv.h"

void Dragon::readIn(FILE *fd) 
{
  int segAddr;
 
  while ((segAddr = processSegment(fd)) != -1)
    ;
}

void Dragon::writeOut(FILE *fd) {
}

int Dragon::processSegment(FILE *fd) {
  int c = fgetc(fd);
  int len = 0;
  int addr = 0;
  int res;
  int i;
  

  if (c == 0xff) {
    len = getWord(fd);
    addr = getWord(fd);

    if ((len != 0) || (addr == -1)) {
      fprintf(stderr, "Badly formatted END segment.\n");
      exit(1);
    }

    res = -1;
  }
  else if (c != 0) {
    fprintf(stderr, "Badly formatted CHUNK type.\n");
    exit(1);
  }
  else {
    len = getWord(fd);
    addr = getWord(fd);

    if ((len == -1) || (addr == -1)) {
      fprintf(stderr, "Badly formatted CHUNK segment.\n");
      exit(1);
    }

    for (i=0; i<len; i++) {
      c = fgetc(fd);

      if (c == EOF) {
	fprintf(stderr, "Unexpected EOF.\n");
	exit(1);
      }

      mem->setByte(addr+i, c);

      res = addr;
    }
  }

  return res;
}

int Dragon::getWord(FILE *fd) {
  int c1 = fgetc(fd);
  int c2 = fgetc(fd);

  if ((c1 == EOF) || (c2 == EOF)) {
    return -1;
  }

  return (c1<<8) | c2;
}

