#ifndef _RAW_H_
#define _RAW_H_

#include <stdio.h>

#include "conv.h"

class Raw : public Converter {
 private:
 protected:
 public:
  const static int RAW_OK = 0;

 private:
 protected:
 public:
  Raw() : Converter() { setName("Raw binary"); };

  void readIn(FILE *fd);
  void writeOut(FILE *fd);
};

#endif
