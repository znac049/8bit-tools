#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "conv.h"

Converter *findConverter(const char *format) {
  if (strcasecmp(format, "srec") == 0) {
    return new MotoSREC();
  }
  else if (strcasecmp(format, "ihex") == 0) {
    return new IntelHex();
  }
  else if (strcasecmp(format, "raw") == 0) {
    return new Raw();
  }
  else if (strcasecmp(format, "dragon") == 0) {
    return new Dragon();
  }

  Utils::abortf("I don't know how to deal with format '%s'", format);

  return NULL;
}

int main(int argc, char *argv[]) {
  Args args(argc, argv);

  char inputName[MAXSTR];
  char outputName[MAXSTR];
  char fromFormat[MAXSTR];
  char toFormat[MAXSTR];

  long memSize = 1<<16; // Default memory space is 64k
  bool verbose = true;

  long relocateTo = -1;
  long startAddress = -1;
  long endAddress = -1;

  Memory *mem;
  Converter *cvIn, *cvOut;
  FILE *inFile, *outFile;

  int numArgs;
  option_t *arg;
  
  parseopt_t mandatoryArgs[] = {
    {"input-file",  Args::requires_argument, inputName,  'i'},
    {"output-file", Args::requires_argument, outputName, 'o'},
    {"from-format", Args::requires_argument, fromFormat, 'f'},
    {"to-format",   Args::requires_argument, toFormat,   't'},
    {NULL,          0,                       NULL,      0}
  }; 
 
  parseopt_t optionalArgs[] = {
    {"memory-size",   Args::requires_argument | Args::numeric_argument, &memSize,       's'},
    {"relocate-to",   Args::requires_argument | Args::numeric_argument, &relocateTo,    'r'},
    {"start-address", Args::requires_argument | Args::numeric_argument, &startAddress,  'r'},
    {"end-address",   Args::requires_argument | Args::numeric_argument, &endAddress,  'r'},
    {"help",          0,                                                &verbose,       '?'},
    {"verbose",       0,                                                NULL,           'v'},
    {NULL,            0,                                                NULL,           0  }
  };

  try {
    args.parseArgs(mandatoryArgs, optionalArgs, false);
  }
  catch (exception e) {
    Utils::abortf("Trouble parsing command line.\n");
  }

  numArgs = args.getArgC(Args::argument);

  if (numArgs != 0) {
    Utils::abortf("No trailing arguments expected.\n");
  }

  inFile = fopen(inputName, "r");
  if (inFile == NULL) {
    Utils::abortf("Couldn't open '%s' - check it exists and is readable.", inputName);
  }

  outFile = fopen(outputName, "w");
  if (outFile == NULL) {
    Utils::abortf("Couldn't create '%s' - check you have permission.", outputName);
  }

  mem = new Memory(memSize);
  mem->setPadChar(0x5a);

  cvIn = findConverter(fromFormat);
  if (verbose) {
    printf("Input file type: %s\n", cvIn->getName());
  }

  cvIn->setVerbose(verbose);
  cvIn->setMemory(mem);
  cvIn->readIn(inFile);
  fclose(inFile);

  if (verbose) {
    mem->info();
  }

  cvOut = findConverter(toFormat);
  if (verbose) {
    printf("Output file type: %s\n", cvOut->getName());
  }

  cvOut->setVerbose(verbose);
  cvOut->setMemory(mem);
  cvOut->setRelocateTo(relocateTo);
  cvOut->setStartAddress(startAddress);
  cvOut->setEndAddress(endAddress);
  cvOut->writeOut(outFile);
  fclose(outFile);

  //mem->dump();

  return 0;
}
