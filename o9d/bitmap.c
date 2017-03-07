#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "o9d.h"

static byte *bitmap = NULL;
static int bitmapSize;

void loadBitmap(FILE *fd)
{
  int numRead;

  if (bitmap != NULL) {
    free(bitmap);
  }

  bitmapSize = numBitmapSectors * sectorSize;

  bitmap = getvec(bitmapSize);
  if (seekLSN(fd, 1) == -1) {
    fprintf(stderr, "Failed to position to read bitmap - giving up!\n");
    exit(1);
  }
  
  numRead = fread(bitmap, 1, bitmapSize, fd);
  if (numRead != bitmapSize) {
    fprintf(stderr, "Failed to read bitmap data - giving up!\n");
    exit(1);
  }
}

int isAllocated(long lsn)
{
  long clusterNumber;
  long byteOffset;
  int bitNum;
  int mask;
  byte mapByte;

  lsn = lsn - (numBitmapSectors + 1);
  if (lsn < 0) {
    return FALSE;
  }

  clusterNumber = lsn/sectorsPerCluster;
  byteOffset = clusterNumber>>3;
  bitNum = 7 - clusterNumber & 0x07;
  mask = 1<<bitNum;
  mapByte = bitmap[byteOffset];

  printf("LSN %ld is cluster %ld. Byte number %ld:0x%02x, bit %d, mask %d\n",
     lsn+numBitmapSectors+1, clusterNumber, byteOffset, bitmap[byteOffset], bitNum, mask);

  return (mapByte & mask)?TRUE:FALSE;
}
