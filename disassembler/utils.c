#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "bda.h"

int is_hex_char(char c) {
  c = tolower(c);

  if ((c >= '0') && (c <= '9')) {
    return YES;
  }

  if ((c >= 'a') && (c <= 'f')) {
    return YES;
  }

  return NO;
}

int hex_val(char c) {
  c = tolower(c);

  if ((c >= '0') && (c <= '9')) {
    return c - '0';
  }

  if ((c >= 'a') && (c <= 'f')) {
    return c - 'a' + 10;
  }

  return -1;
}

unsigned short hex2int(char *str, unsigned short dfl) {
  int res = 0;

  while (*str) {
    if (!is_hex_char(*str)) {
      return dfl;
    }

    res = (res << 4) | hex_val(*str);
    str++;
  }

  return res;
}

unsigned short parse_address(char *str) {
  int res = 0xffff;

  if ((strncmp(str, "0x", 2) == 0) || (strncmp(str, "0X", 2) == 0)) {
    res = hex2int(&str[2], 0xffff);
  }
  else if (strncmp(str, "$", 1) == 0) {
    res = hex2int(&str[1], 0xffff);
  }
  else {
    res = atoi(str);
  }

  return res;
}

unsigned char is_file(char *path) {
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

char *trunc_str(char *s, char c) {
  char *ch = strchr(s, c);

  if (ch != NULL) {
    *ch = '\0';
  }

  return s;
}

char *strip_nl(char *str) {
  int len = strlen(str);

  if (str[len-1] == '\n') {
    str[len-1] = EOS;
    len--;
  }

  if (str[len-1] == '\r') {
    str[len-1] = EOS;
  }

  return str;
}

void ltrim(char *s) {
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
  }
}

void rtrim(char *s) {
  int len = strlen(s)-1;
  int i = len;

  while ((i >= 0) && IS_WHITE(s[i])) {
    i--;
  }

  if (i < len) {
    s[i+1] = '\0';
  }
}

void trim(char *s) {
  ltrim(s);
  rtrim(s);
}

char *str_pop(char **str) {
  char *res = *str;
  char *cp = res;

  while (*cp) {
    if (IS_WHITE(*cp)) {
      *str = cp;
      return res;
    }

    cp++;
  }

  *str = cp;
  return res;
}

char *skip_blanks(char *str) {
  while (IS_WHITE(*str)) {
    str++;
  }

  return str;
}
