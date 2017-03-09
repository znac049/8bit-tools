#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define COLOUR_STRINGS
#include "o9d.h"

static int isTTY=0;
colour_t *fgColours;
static int rows;
static int cols;

void initColourEngine()
{
  struct winsize w;
  int res = ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  if (res == -1) {
    printf("ioctl() failed!\n");

    rows = cols = -1;
    isTTY = 0;
  }

  rows = w.ws_row;
  cols = w.ws_col;

  /*
  printf("console=%d X %d\n", rows, cols);
  printf("sectorSize=%d\n", sectorSize);
  printf("sizeof(colour_t)=%ld\n", sizeof(colour_t));
  */

  isTTY = isatty(fileno(stdout));

  fgColours = getvec(sectorSize * sizeof(colour_t));
}

const char *getColourString(colour_t colour)
{
  return colours[colour];
}

void setColour(int offset, int count, colour_t colour)
{
  int i;
  
  if ((offset < 0) || (offset+count >= sectorSize)) {
    return;
  }

  for (i = 0; i < count; i++) {
    fgColours[offset+i] = colour;
  }
}

void changeColour(colour_t colour)
{
  static colour_t currentColour = RESET;

  if (isTTY && (currentColour != colour)) {
    fputs(getColourString(colour), stdout);
    currentColour = colour;
  }
}
