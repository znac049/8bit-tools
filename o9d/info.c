#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>

#include "o9d.h"

static char **lines;
static int numLines = 0;

void allocInfoLines()
{
  int i;

  numLines = sectorSize / BYTES_PER_LINE;
  lines = getvec(numLines * sizeof(char *));

  for (i = 0; i < numLines; i++) {
    lines[i] = getvec(MAXSTR+1);
  }
}

void clearInfoLines()
{
  int i;

  for (i = 0; i < numLines; i++) {
    *lines[i] = '\0';
  }
}

char *getInfoLine(int lineNum)
{
  if ((lineNum < 0) || (lineNum >= numLines)) {
    return "";
  }

  return lines[lineNum];
}

void setInfoLine(int lineNum, const char *fmt, ...)
{
  va_list args;

  if ((lineNum < 0) || (lineNum >= numLines)) {
    return;
  }

  va_start(args, fmt);
  vsnprintf(lines[lineNum], MAXSTR, fmt, args);
  va_end(args);
}
