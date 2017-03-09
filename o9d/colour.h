#ifndef _COLOUR_H_
#define _COLOUR_H_

enum Colour {
  RESET=0, 
  WHITE, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN,
  BRIGHTWHITE, BRIGHTRED, BRIGHTGREEN, BRIGHTYELLOW, BRIGHTBLUE, BRIGHTMAGENTA, BRIGHTCYAN
};
typedef enum Colour colour_t;

#ifdef COLOUR_STRINGS
static char *colours[] = {
  "\x1b[0m",    // Reset
  "\x1b[0;31m", // White
  "\x1b[0;32m", // Red
  "\x1b[0;33m", // Green
  "\x1b[0;34m", // Yellow
  "\x1b[0;35m", // Blue
  "\x1b[0;36m", // Magenta
  "\x1b[1;37m", // Cyan
  "\x1b[1;31m", // Bright White
  "\x1b[1;32m", // Bright Red
  "\x1b[1;33m", // Bright Green
  "\x1b[1;34m", // Bright Yellow
  "\x1b[1;35m", // Bright Blue
  "\x1b[1;36m", // Bright Magenta
  "\x1b[1;37m"  // Bright Cyan
};
#endif

#endif
