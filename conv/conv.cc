#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <getopt.h>

#include "conv.h"

#define no_argument 0
#define required_argument 1 
#define optional_argument 2

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
    char inputName[MAXSTR];
    char outputName[MAXSTR];
    char fromFormat[MAXSTR];
    char toFormat[MAXSTR];

    long memSize = 1<<16; // Default memory space is 64k
    bool verbose = false;

    long relocateTo = -1;
    long startAddress = -1;
    long endAddress = -1;

    int index;
    int iarg=0;

    Memory *mem;
    Converter *cvIn, *cvOut;
    FILE *inFile, *outFile;

      const struct option longopts[] =
    {
        {"input-file",    required_argument, 0, 'i'},
        {"output-file",   required_argument, 0, 'o'},
        {"from-format",   required_argument, 0, 'f'},
        {"to-format",     required_argument, 0, 't'},
        {"memory-size",   required_argument, 0, 's'},
        {"relocate-to",   required_argument, 0, 'r'},
        {"start-address", required_argument, 0, 'S'},
        {"end-address",   required_argument, 0, 'E'},
        {"verbose",       no_argument,       0, 'v'},
        {0,0,0,0},
    };

    //turn off getopt error message
    opterr=1; 

    strcpy(inputName, "");
    strcpy(outputName, "");
    strcpy(fromFormat, "");
    strcpy(toFormat, "");

    while((iarg = getopt_long(argc, argv, "i:o:f:t:s:r:S:E:v", longopts, &index)) != -1)
    {
        switch (iarg)
        {
            case 'i':
                strcpy(inputName, optarg);
                break;
                
            case 'o':
                strcpy(outputName, optarg);
                break;
                
            case 'f':
                strcpy(fromFormat, optarg);
                break;
                
            case 't':
                strcpy(toFormat, optarg);
                break;
                
            case 's':
                memSize = Utils::parseAddress(optarg);
                break;
                
            case 'r':
                relocateTo = Utils::parseAddress(optarg);
                break;
                
            case 'S':
                startAddress = Utils::parseAddress(optarg);
                break;

            case 'E':
                endAddress = Utils::parseAddress(optarg);
                break;

            case 'v':
                verbose = true;
                break;

            default:
                printf("WTF!\n");
                break;
        }
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

    if (startAddress == -1) {
        startAddress = mem->getLowestAddress();
    }

    if (endAddress == -1) {
        endAddress = mem->getHighestAddress();
    }

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
