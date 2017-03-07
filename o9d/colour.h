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
  "\x1b[0m", "\x1b[0;31m", "\x1b[0;32m", "\x1b[0;33m", "\x1b[0;34m", "\x1b[0;35m", "\x1b[0;36m",
  "\x1b[1;37m", "\x1b[1;31m", "\x1b[1;32m", "\x1b[1;33m", "\x1b[1;34m", "\x1b[1;35m", "\x1b[1;36m"
};
#endif

#endif
