#ifndef _INTELHEX_H_
#define _INTELHEX_H_

#include <stdio.h>

#include "conv.h"

class IntelHex : public Converter {
 private:
 protected:
 public:
  const static int IHEX_OK = 0;

 private:
  int convert(const char *str, int nChars);
  int process(const char *line);

 protected:
 public:
 IntelHex() : Converter() { setName("Intel HEX"); };

  void readIn(FILE *fd);
  void writeOut(FILE *fd);
};

#endif
