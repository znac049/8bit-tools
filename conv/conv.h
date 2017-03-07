#ifndef _BDA_H_
#define _BDA_H_

#define MAXSTR 512

#define YES true
#define NO false

#define CONTINUE_CHAR '\\'
#define EOS '\0'

#include "Args.h"
#include "Exceptions.h"
#include "MemoryCell.h"
#include "Memory.h"
#include "Utils.h"
#include "Converter.h"
#include "MotoSREC.h"
#include "IntelHex.h"
#include "Raw.h"

enum memoryType {
  MEM_BYTE, MEM_CODE, MEM_WORD, MEM_EMPTY, MEM_UNKNOWN
};

#endif
