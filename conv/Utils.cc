#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>

#include "conv.h"

#define IS_WHITE(c) ((c == ' ') || (c == '\t'))

bool Utils::isHexChar(char c) {
  c = tolower(c);

  if ((c >= '0') && (c <= '9')) {
    return YES;
  }

  if ((c >= 'a') && (c <= 'f')) {
    return YES;
  }

  return NO;
}

int Utils::hexVal(char c) {
  c = tolower(c);

  if ((c >= '0') && (c <= '9')) {
    return c - '0';
  }

  if ((c >= 'a') && (c <= 'f')) {
    return c - 'a' + 10;
  }

  return -1;
}

long Utils::hex2int(char *str) {
  int res = 0;

  while (*str) {
    if (!isHexChar(*str)) {
      throw ConversionException();
    }

    res = (res << 4) | hexVal(*str);
    str++;
  }

  return res;
}

long Utils::parseAddress(char *str) {
    long res;

    if (str == NULL) {
        return -1;
    }

    // printf("parseAddress('%s')\n", str);
    try {
        if ((strncmp(str, "0x", 2) == 0) || (strncmp(str, "0X", 2) == 0)) {
            res = hex2int(&str[2]);
        }
        else if (strncmp(str, "$", 1) == 0) {
            res = hex2int(&str[1]);
        }
        else {
            res = atol(str);
        }
    }
    catch (ConversionException e) {
        res = -1;
    }

    // printf("  -> %ld (0x%08x)\n", res, res);
    return res;
}

bool Utils::isFile(const char *path) {
  struct stat inf;
  int res = stat(path, &inf);

  if (res) {
    return NO;
  }

  if (S_ISREG(inf.st_mode)) {
    return YES;
  }

  return NO;
}

char *Utils::truncStr(char *s, char c) {
  char *ch = strchr(s, c);

  if (ch != NULL) {
    *ch = '\0';
  }

  return s;
}

void Utils::ltrim(char *s) {
  char *sp = s;

  if (IS_WHITE(*sp)) {
    /* Leading space(s) */
    int i;
    int j;

    while (IS_WHITE(*sp)) {
      sp++;
    }

    while (*sp) {
      *s++ = *sp++;
    }
    *s = EOS;
  }
}

void Utils::rtrim(char *s) {
  int len = strlen(s)-1;
  int i = len;

  while ((i >= 0) && IS_WHITE(s[i])) {
    i--;
  }

  if (i < len) {
    s[i+1] = '\0';
  }
}

void Utils::trim(char *s) {
  ltrim(s);
  rtrim(s);
}

char Utils::printableChar(char ch) {
  if ((ch < ' ') || (ch > '~')) {
    return '.';
  }

  return ch;
}

void Utils::abortf(const char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);

  exit(1);
}
