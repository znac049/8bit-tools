#ifndef _CONVERTER_H_
#define _CONVERTER_H_

#include <stdio.h>

#include "conv.h"

class Converter {
 private:
 protected:
  Memory *mem;

  long relocateTo;
  char name[MAXSTR];

  long startAddress;
  long endAddress;

 public:

 private:
 protected:
 public:
  Converter();

  virtual void readIn(FILE *fd) {};
  virtual void writeOut(FILE *fd) {};

  void setName(const char *newName);
  const char *getName();

  void setMemory(Memory *memory);
  void setRelocateTo(long addr);

  void setStartAddress(long addr);
  void setEndAddress(long addr);

  long getStartAddress();
  long getEndAddress();
};

#endif
