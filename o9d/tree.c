#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>

#include "o9d.h"

char *decodeFilename(const char *name)
{
  static char decodedName[MAXSTR];
  char *str = decodedName;
  int i = 0;

  while ((*name & 0x80) == 0) {
    i++;
    if (i >= MAXSTR) {
      decodedName[MAXSTR-1] = '\0';
      return decodedName;
    }

    *str++ = *name++;
  }

  *str++ = *name & 0x7f;
  *str = '\0';
  return decodedName;
}

void checkLSN(FILE *fd, long lsn)
{
  // Read the file descriptor sector and see if it's a directory.
  byte des[MAX_SECTOR_SIZE];
  filedes_t *fdes = (filedes_t *)des;

  readLSNInto(fd, lsn, des);
  if (fdes->attribs & 0x80) {
    // printf("Directory! LSN%ld -> DIR\n", lsn);
    sectorType[lsn] = DIR;
  }
  else {
    //printf("File.\n");
  }
}

void trawlSeg(FILE *fd, seg_t *seg)
{
  byte ents[MAX_SECTOR_SIZE];
  dirent_t *ent = (dirent_t *)ents;
  int i;
  int entsPerSector = sectorSize / sizeof(dirent_t);

  //printf("Look at dirents in LSN%ld\n", decodeLSN(seg->lsn));

  readLSNInto(fd, decodeLSN(seg->lsn), ents);
  for (i = 0; i < entsPerSector; i++) {
    //printf("DirENT %d: '%s' - @%ld\n", i, decodeFilename(ent->name), decodeLSN(ent->desLSN));
    checkLSN(fd, decodeLSN(ent->desLSN));

    ent++;
  }
}

void trawlDirectories(FILE *fd, long lsn)
{
  long dotLSN;
  long dotdorLSN;
  seg_t *seg;
  int i;

  filedes_t *fdes = (filedes_t *)readLSN(fd, lsn);

  // printf("\n\nTrawling:\n");

  // The attribs should identify us as a directory, otherwise
  // somethings badly wrong!
  if (!fdes->attribs & 0x80) {
    fprintf(stderr, "Panic! LSN should be a directory descriptor, but it is a file instead.\n");
    exit(1);
  }

  // Okay trunc through the dirents until we run out or get an
  // LSN of 0
  seg = (seg_t *)fdes->segments;
  for (i = 0; i < 48; i++) {
    long segLSN = decodeLSN(seg->lsn);

    if (segLSN == 0) {
      return;
    }

    sectorType[segLSN] = DIRENT;
    trawlSeg(fd, seg);

    seg++;
  }
}

