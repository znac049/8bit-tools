#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdarg.h>

#include "o9d.h"

static struct option long_options[] = {
  {"sector-size", required_argument, NULL, 'S'},
  {"low-lsn", required_argument, NULL, 'l'},
  {"high-lsn", required_argument, NULL, 'h'},
  {"absolute-offsets", no_argument, NULL, 'a'},
  {"relative-offsets", no_argument, NULL, 'r'},
  {"verbode", no_argument, NULL, 'v'},
  {NULL, 0, NULL, 0}
};

int sectorSize = 256;

long lowestLSN = 0;
long highestLSN = -1;

int relativeOffsets = FALSE;

int termWidth;
int colorize = FALSE;

char *filename = NULL;

byte *sectorBuffer;
sector_type_t *sectorType;
// colour_t *fgColours;

long totalSectors = 1;
word sectorsPerTrack = 1;
long rootLSN = 0;
long bootLSN = 0;
word bootLen = 0;
word mapSize = 1;
word sectorsPerCluster = 1;
int numBitmapSectors = 1;

void usage(const char *fmt, ...)
{
  va_list args;
  int len = strlen(fmt);

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  if (len && (fmt[len-1] != '\n')) {
    fprintf(stderr, "\n");
  }

  fprintf(stderr, "\nUsage:\n");

  exit(1);
}

void decodeDD(FILE *fd)
{
  struct DiskDescriptor *dd = (struct DiskDescriptor *) sectorBuffer;
  int numRead;
  int i;

  rewind(fd);
  numRead = fread(sectorBuffer, 1, sectorSize, fd);

  if (numRead != sectorSize) {
    fprintf(stderr, "Failed to read sector 0 - bad, bad, bad...\n");
    exit(1);
  }

  totalSectors = decodeLSN(dd->totalSectors);
  sectorsPerTrack = decodeLSN(dd->sectorsPerTrack);
  rootLSN = decodeLSN(dd->rootLSN);
  bootLSN = decodeLSN(dd->bootLSN);
  bootLen = decodeWord(dd->bootSize);
  mapSize = decodeWord(dd->map);
  sectorsPerCluster = decodeWord(dd->sectorsPerCluster);

  numBitmapSectors = (mapSize / sectorSize) + 1;

  sectorType = getvec(sizeof(sector_type_t) * totalSectors);
  sectorType[0] = DISKDES;
  for (i = 0; i < numBitmapSectors; i++) {
    sectorType[1+i] = BITMAP;
  }

  for (i = numBitmapSectors+1; i < totalSectors; i++) {
    sectorType[i] = DATA;
  }

  trawlDirectories(fd, rootLSN);
}

int dumpLSN(FILE *fd, long lsn)
{
  int offset;
  long addr = 0;
  int numRead;
  int lineNum;
  int i;
  char banner[512];
  int allocated = isAllocated(lsn);

  if (seekLSN(fd, lsn) == -1) {
    return -1;
  }

  numRead = fread(sectorBuffer, 1, sectorSize, fd);
  if (numRead != sectorSize) {
    return -1;
  }

  clearInfoLines();
  for (i = 0; i < sectorSize; i++) {
    fgColours[i] = RESET;
  }

  snprintf(banner, sizeof(banner), "LSN #%ld:", lsn);
  if (relativeOffsets) {
    snappend(banner, sizeof(banner), " (@offset %06lx)", lsn*sectorSize);
  }
  printf("%-58s", banner);

  // If it's a special sector, print extra inf
  if (lsn == 0) {
    // LSN 0 describes the filesystem and disk layout
    dumpDD();
  }
  else if ((lsn >= 1) && (lsn <= numBitmapSectors)) {
    dumpBitmapSector(lsn);
  }
  else if (lsn == rootLSN) {
    dumpFD("Root Directory");
  }
  else if (allocated) {
    // The bitmap indicates this sector is allocated
    if (sectorType[lsn] == DIR) {
      // Directory entries
      dumpFD("Directory");
    }
    else if (sectorType[lsn] == DIRENT) {
      printf("%sDirent%s", "\x1b[1m", getColourString(RESET));
      setColour(0, sectorSize, BRIGHTMAGENTA);
    }
    else {
      // Simple data block
      printf("%sAllocated sector%s", "\x1b[1m", getColourString(RESET));
      setColour(0, sectorSize, YELLOW);
    }
  }
  else {
    printf("%sUnallocated sector%s", "\x1b[1m", getColourString(RESET));
    setColour(0, sectorSize, BLUE);
  }

  printf("\n");

  if (!relativeOffsets) {
    addr = lsn * sectorSize;
  }

  for (offset = 0, lineNum = 0; offset < sectorSize; offset += BYTES_PER_LINE, lineNum++) {
    int i;

    changeColour(GREEN);
    printf("%06lx ", addr + offset);

    for (i = 0; i < BYTES_PER_LINE; i++) {
      changeColour(fgColours[offset+i]);
      printf("%02x ", sectorBuffer[offset+i]);
    }

    changeColour(RESET);
    printf("   ");

    for (i = 0; i < BYTES_PER_LINE; i++) {
      changeColour(fgColours[offset+i]);
      printf("%c", printable(sectorBuffer[offset+i]));
    }

    changeColour(RESET);
    printf("   %s", getInfoLine(lineNum));

    printf("\n");
  }

  printf("\n");

  return 0;
}

void doDump()
{
  FILE *fd = fopen(filename, "r");
  long endLSN;
  long lsn;

  if (fd == NULL) {
    fprintf(stderr, "Can't open disk image file '%s'\n", filename);
    exit(1);
  }

  initColourEngine();
  allocInfoLines();

  sectorBuffer = getvec(sectorSize);
  fgColours = getvec(sectorSize * sizeof(enum Colour));


  // Calculate actual highest LSN in file
  fseek(fd, 0, SEEK_END);
  endLSN = (ftell(fd) / sectorSize) - 1;
  
  if (lowestLSN > endLSN) {
    lowestLSN = endLSN;
  }

  if (highestLSN == -1) {
    highestLSN = endLSN;
  }
  else if (highestLSN > endLSN) {
    highestLSN = endLSN;
  }

  // Pre read the DD sector and the bitmap
  decodeDD(fd);
  loadBitmap(fd);

  printf("Dumping LSN %ld to %ld\n", lowestLSN, highestLSN);

  for (lsn = lowestLSN; lsn <= highestLSN; lsn++) {
    if (dumpLSN(fd, lsn)) {
      fprintf(stderr, "Failed to dump LSN %ld - aborting\n", lsn);
      fclose(fd);
      return;
    }
  }

  fclose(fd);
}

int main(int argc, char **argv)
{
  int opt;
  int val;
  int linesPerSector;
  int i;
  char argPresent[128];

  bzero(argPresent, sizeof(argPresent));

  while ((opt = getopt_long(argc, argv, "S:l:h:arv", long_options, NULL)) != -1) {
    argPresent[opt] = TRUE;

    switch (opt) {
    case 'S':
      if (!is_number(optarg)) {
	usage("sector size must be numeric and a power of 2 ('%s')", optarg);
      }
      val = atol(optarg);
      if ((val == 128) || (val == 256) || (val == 512) || (val == 1024)) {
	sectorSize = val;
      }
      else {
	usage("Invalid value for sector size (%s)", optarg);
      }
      break;

    case 'l':
      if (!is_number(optarg)) {
	usage("low-lsn must be numeric ('%s')", optarg);
      }
      lowestLSN = atol(optarg);
      break;

    case 'h':
      if (!is_number(optarg)) {
	usage("high-lsn must be numeric ('%s')", optarg);
      }
      highestLSN = atol(optarg);
      break;

    case 'r':
      relativeOffsets = TRUE;
      break;

    case 'a':
      relativeOffsets = FALSE;
      break;

    default:
      usage("Bad args!");
      break;
    }
  }

  if (argPresent['a'] && argPresent['r']) {
    usage("relative-offssets (r) and absolute-offsets (a) are mutually exclusive");
  }

  if (optind == argc) {
    usage("missing filename");
  }
  else if ((argc - optind) != 1) {
    usage("Only one trailing argument allowed.");
  }

  filename = strdup(argv[argc-1]);

  doDump();
}
