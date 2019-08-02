#ifndef _INTELHEX_H_
#define _INTELHEX_H_

#include <stdio.h>

#include "conv.h"

class IntelHex : public Converter {
 private:
 protected:
 public:
  const static int IHEX_OK = 0;
  const static int IHEX_BAD = 1;
  const static int IHEX_UNKNOWNTYPE = 2;
  const static int IHEX_UNIMPLEMENTED = 3;
  

 private:
  int convert(const char *str, int nChars);
  int checksum(const char *line);
  int process(const char *line);

 protected:
 public:
 IntelHex() : Converter() { setName("Intel HEX"); };

  void readIn(FILE *fd);
  void writeOut(FILE *fd);
};

#endif
