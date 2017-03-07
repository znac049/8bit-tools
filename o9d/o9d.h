#ifndef __O9D_H__
#define __O9D_H__

#ifndef TRUE
# define TRUE (1)
# define FALSE (0)
#endif

#define MAX_SECTOR_SIZE 1024
#define MIN_SECTOR_SIZE 128

#define MAXSTR 256

#define BYTES_PER_LINE 16

#include "colour.h"

enum SectorType {
  UNKNOWN=0, DISKDES, BITMAP, FILEDES, DATA, DIR, DIRENT
};
typedef enum SectorType sector_type_t;

typedef unsigned char byte;
typedef unsigned short int word;

struct DiskDescriptor {
  byte totalSectors[3];
  byte numTracks;
  byte map[2];
  byte sectorsPerCluster[2];
  byte rootLSN[3];
  byte owner[2];
  byte diskAttr;
  byte diskId[2];
  byte diskFormat;
  byte sectorsPerTrack[2];
  byte res1[2];
  byte bootLSN[3];
  byte bootSize[2];
  byte created[5];
  char volName[32];
  byte options[32];
};
typedef struct DiskDescriptor diskdes_t;

struct FileDescriptor {
  byte attribs;
  byte owner[2];
  byte modified[5];
  byte numLinks;
  byte filesize[4];
  byte created[3];
  byte segments[240];
};
typedef struct FileDescriptor filedes_t;

struct DirEnt {
  byte name[29];
  byte desLSN[3];
};
typedef struct DirEnt dirent_t;

struct Segment {
  byte lsn[3];
  byte blockSize[2];
};
typedef struct Segment seg_t;


/* Global variables */
extern int sectorSize;

extern long lowestLSN;
extern long highestLSN;

extern int relativeOffsets;

extern byte *sectorBuffer;
extern sector_type_t *sectorType;
extern colour_t *fgColours;

extern long totalSectors;
extern word sectorsPerTrack;
extern long rootLSN;
extern long bootLSN;
extern word bootLen;
extern word mapSize;
extern word sectorsPerCluster;
extern int numBitmapSectors;

/* bitmap.c */
void loadBitmap(FILE *fd);
int isAllocated(long lsn);

/* colour.c */
void initColourEngine();
void changeColour(colour_t colour);
const char *getColourString(colour_t colour);
void setColour(int offset, int count, colour_t colour);

/* dump.c */
void dumpDD();
void dumpBitmapSector(long lsn);
void dumpFD(const char *heading);
void dumpDirent();

/* info.c */
void allocInfoLines(int nLines);
void clearInfoLines();
char *getInfoLine(int lineNum);
void setInfoLine(int lineNum, const char *fmt, ...);

/* utils.c */
void abortf(const char *fmt, ...);
const char *collapseVolumeName(const char *str);
void decodeAttributes(char *buf, size_t len, byte attr);
long decodeLong(const byte *lsn);
long decodeLSN(const byte *lsn);
word decodeWord(const byte *ptr);
void *getvec(size_t size);
int is_number(const char *str);
char printable(char c);
const char *printableString(const char *str);
char *readLSNInto(FILE *fd, long lsn, byte *buff);
char *readLSN(FILE *fd, long lsn);
long seekLSN(FILE *fd, long lsn);
int valid_digit(char dig, int radix);

#endif
