#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>

#include "o9d.h"

void dumpDD() 
{
  struct DiskDescriptor *dd = (struct DiskDescriptor *) sectorBuffer;
  int lineNum = 0;

  const char *resetC   = getColourString(RESET);
  const char *blueC    = getColourString(BLUE);
  const char *yellowC  = getColourString(YELLOW);
  const char *redC     = getColourString(RED);
  const char *greenC   = getColourString(GREEN);
  const char *cyanC    = getColourString(CYAN);
  const char *magentaC = getColourString(MAGENTA);

  printf("%sDisk Descriptor:%s", "\x1b[1m", resetC);

  setInfoLine(lineNum++, "Volume: %s'%s'%s", redC, collapseVolumeName(dd->volName), resetC);
  setInfoLine(lineNum++, "Created: %s%d-%02d-%02d, %02d:%02d%s",
	      magentaC, 1900+dd->created[0], dd->created[1], dd->created[2], 
	      dd->created[3], dd->created[4], resetC); 

  lineNum++;
  setInfoLine(lineNum++, "#Secs: %s%ld%s, #Trks: %d, #SPT: %d", yellowC, totalSectors, resetC, dd->numTracks, sectorsPerTrack);
  setInfoLine(lineNum++, "Root LSN: %s%ld%s", yellowC, rootLSN, resetC);
  setInfoLine(lineNum++, "Boot LSN: %s%ld%s, size: %s%d%s", yellowC, bootLSN, resetC, cyanC, bootLen, resetC);

  lineNum++;
  setInfoLine(lineNum++, "Map size: %s%d%s (%s%d%s sectors)", greenC, mapSize, resetC, greenC, numBitmapSectors, resetC);
  setInfoLine(lineNum++, "#Secs/cluster: %s%d%s", blueC, sectorsPerCluster, resetC);

  // Set the bytes to highlight in different colours
  
  // LSNs in yellow
  setColour(0, 3, YELLOW);
  setColour(8, 3, YELLOW);
  setColour(0x15, 3, YELLOW);

  // Allocation map
  setColour(4, 2, GREEN);

  // Sectors per cluster
  setColour(6, 2, BLUE);

  // Volume string
  setColour(0x1f, 32, RED);

  // Boot size
  setColour(0x18, 2, CYAN);

  // Datestamp
  setColour(0x1a, 5, MAGENTA);
}

void dumpBitmapSector(long lsn)
{
  printf("%sBitmap sector %ld of %d%s", "\x1b[1m", lsn, numBitmapSectors, getColourString(RESET));
  setColour(0, sectorSize, CYAN);
}

void dumpFD(const char *heading)
{
  struct FileDescriptor *fd = (struct FileDescriptor *) sectorBuffer;
  int lineNum = 0;
  long fileSize = decodeLong(fd->filesize);
  char attribString[16];
  int i;

  const char *resetC   = getColourString(RESET);
  const char *blueC    = getColourString(BLUE);
  const char *yellowC  = getColourString(YELLOW);
  const char *redC     = getColourString(RED);
  const char *greenC   = getColourString(GREEN);
  const char *cyanC    = getColourString(CYAN);
  const char *magentaC = getColourString(MAGENTA);

  if (heading != NULL) {
    printf("%s%s%s", "\x1b[1m", heading, getColourString(RESET));
  }
  else {
    printf("%sFile Descriptor%s", "\x1b[1m", getColourString(RESET));
  }

  decodeAttributes(attribString, sizeof(attribString), fd->attribs);

  setInfoLine(lineNum++, "Attrs: %s%s%s", blueC, attribString, resetC);
  setInfoLine(lineNum++, "Created: %s%d-%02d-%02d%s", greenC, 
	      1900+fd->created[0], fd->created[1], fd->created[2], resetC); 

  setInfoLine(lineNum++, "Modified: %s%d-%02d-%02d, %02d:%02d%s",
	      magentaC, 1900+fd->modified[0], fd->modified[1], fd->modified[2], 
	      fd->modified[3], fd->modified[4], resetC); 

  setInfoLine(lineNum++, "Size: %s%ld%s", yellowC, fileSize, resetC);

  setInfoLine(lineNum++, "Link count: %s%d%s", cyanC, fd->numLinks, resetC);

  setColour(0, 1, BLUE);
  setColour(3, 5, MAGENTA);
  setColour(8, 1, CYAN);
  setColour(9, 4, YELLOW);
  setColour(13, 3, GREEN);

  for (i = 16; i < sectorSize; i+= 20) {
    setColour(i, 3, BRIGHTRED);
    setColour(i+3, 2, RED);

    setColour(i+5, 3, BRIGHTYELLOW);
    setColour(i+8, 2, YELLOW);

    setColour(i+10, 3, BRIGHTCYAN);
    setColour(i+13, 2, CYAN);

    setColour(i+15, 3, BRIGHTGREEN);
    setColour(i+18, 2, GREEN);
  }
}

void dumpDirent() {
}

