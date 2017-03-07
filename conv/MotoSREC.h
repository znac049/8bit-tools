#ifndef _MOTOSREC_H_
#define _MOTOSREC_H_

#include <stdio.h>

#include "conv.h"

class MotoSREC : public Converter {
 private:
 protected:
 public:
  const static int SREC_OK = 0;
  const static int SREC_TOOSHORT = 1;
  const static int SREC_BADFORMAT = 2;
  const static int SREC_UNIMPLEMENTED = 3;
  const static int SREC_CHECKSUM = 4;

 private:
  int checksum(const char *line);
  int convert(const char *str, int nChars);
  int process(const char *line);

 protected:
 public:
 MotoSREC() : Converter() { setName("Motorola S-Record"); };

  void readIn(FILE *fd);
  void writeOut(FILE *fd);
};

#endif
