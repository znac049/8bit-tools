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
static int *fgColours;
static int rows;
static int cols;

void initColourEngine()
{
  struct winsize w;
  int res = ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  if (res == -1) {
    printf("ioctl() failed!\n");
    return -1;
  }

  rows = w.ws_row;
  cols = w.ws_col;

  printf("console=%d X %d\n", rows, cols);

  isTTY = isatty(fileno(stdout));

  fgColours = getvec(sectorSize * sizeof(colout_t));
}

const char *getColourString(colour_t colour)
{
  return colours[colour];
}

void setColour(int offset, int count, colour_t colour)
{
  int i;

  if ((offset < 0) || (offset+count > sectorSize)) {
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
