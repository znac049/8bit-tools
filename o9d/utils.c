#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>

#include "o9d.h"

static char *tmpstr;
static int tmplen = -1;

void abortf(const char *fmt, ...)
{
  va_list args;

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  fprintf(stderr, "Aborting.\n");
  exit(1);
}

long decodeLong(const byte *lsn)
{
  long res = (lsn[0] << 24) | (lsn[1] << 16) | (lsn[2] << 8) | lsn[3];

  return res;
}

long decodeLSN(const byte *lsn)
{
  long res = (lsn[0] << 16) | (lsn[1] << 8) | lsn[2];

  return res;
}

word decodeWord(const byte *ptr)
{
  return (ptr[0] << 8) | ptr[1];
}

void decodeAttributes(char *buf, size_t len, byte attr)
{
  if (len < 9) {
    strcpy(buf, "Error!");
  }
  else {
    *buf++ = (attr&0x80)?'d':'f';
    *buf++ = (attr&0x40)?'s':'-';

    *buf++ = (attr&0x01)?'r':'-';
    *buf++ = (attr&0x02)?'w':'-';
    *buf++ = (attr&0x04)?'x':'-';

    *buf++ = (attr&0x08)?'r':'-';
    *buf++ = (attr&0x10)?'w':'-';
    *buf++ = (attr&0x20)?'x':'-';

    *buf = '\0';
  }
}

long seekLSN(FILE *fd, long lsn)
{
  long offset = lsn * sectorSize;

  return fseek(fd, offset, SEEK_SET);
}

char *readLSNInto(FILE *fd, long lsn, byte *buff) {
  int numRead;

  if (seekLSN(fd, lsn) == -1) {
    fprintf(stderr, "readLSN(), failed to position image for lsn %ld\n", lsn);
    exit(1);
  }

  numRead = fread(buff, 1, sectorSize, fd);
  if (numRead != sectorSize) {
    fprintf(stderr, "readLSN(), failed to read data for LSN %ld\n", lsn);
    exit(1);
  }

  return buff;
}

char *readLSN(FILE *fd, long lsn) {
  return readLSNInto(fd, lsn, sectorBuffer);
}

int valid_digit(char dig, int radix)
{
  if (radix == 2) {
    return ((dig == '0') || (dig == '1'));
  }

  if (radix == 8) {
    return ((dig >= '0') && (dig <= '7'));
  }
  else if (radix == 10) {
    return ((dig >= '0') && (dig <= '9'));
  }

  if ((dig >= '0') && (dig <= '9')) {
    return 1;
  }

  dig = tolower(dig);
  if ((dig >= 'a') && (dig <= 'f')) {
    return 1;
  }

  return 0;
}

int is_number(const char *str)
{
  int radix = 10;
  
  while (*str) {
    if (!valid_digit(*str, radix)) {
      return 0;
    }

    str++;
  }

  return 1;
}

char printable(char c)
{
  if ((c < ' ') || (c > 0x7f)) {
    return '.';
  }

  return c;
}

void allocStringSpace(const char *str)
{
  int len = strlen(str);

  if (len > tmplen) {
    if (tmplen != -1) {
      free(tmpstr);
    }

    if (len < 100) {
      len = 100;
    }

    tmplen = len;
    tmpstr = getvec(tmplen+1);
  }
}

const char *printableString(const char *str)
{
  char *sp;

  allocStringSpace(str);

  sp = tmpstr;
  while (*str) {
    *sp++ = printable(*str);
    str++;
  }
  *sp = '\0';

  return tmpstr;
}

const char *collapseVolumeName(const char *str)
{
  char *sp;

  allocStringSpace(str);

  sp = tmpstr;
  while (*str) {
    char c = *str & 0x7f;

    if ((c >= ' ') && (c <=0x7f)) {
      *sp++ = c;
    }

    str++;
  }
  *sp = '\0';

  return tmpstr;
}

void *getvec(size_t size)
{
  void *mem = malloc(size);

  if (!mem) {
    fprintf(stderr, "Out of memory error!\n");
    exit(1);
  }

  return mem;
}

void snappend(char *str, size_t size, const char *fmt, ...)
{
  va_list args;
  int len = strlen(str);
  char *cp = &str[len];

  va_start(args, fmt);
  vsnprintf(cp, size-len, fmt, args);
  va_end(args);
}
