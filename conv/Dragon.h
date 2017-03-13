#ifndef _DRAGON_H_
#define _DRAGON_H_

#include <stdio.h>

#include "conv.h"

class Dragon : public Converter {
 private:
 protected:
 public:

 private:
  int processSegment(FILE *fd);
  int getWord(FILE *fd);

 protected:
 public:
 Dragon() : Converter() { setName("Dragon 32/64 binary"); };

  void readIn(FILE *fd);
  void writeOut(FILE *fd);
};

#endif
