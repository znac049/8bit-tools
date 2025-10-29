#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "conv.h"

Args::Args(int argc, char **argv) {
  int i = 1;
  option_t *opt;
  char *slash;

  shortName = longName = strdup(argv[0]);
  slash = strrchr(longName, '/');
  if (slash != NULL) {
    shortName = ++slash;
  }

  while (i < argc) {
    char *argStr = argv[i];

    //printf("Processing arg '%s'\n", argStr);

    if (strncmp(argStr, "--", 2) == 0) {
      char arg[MAXSTR];
      char *eq;
      int flags = 0;

      //printf(" long\n");
      
      opt = new option_t();
      strcpy(arg, argv[i]);

      eq = strchr(arg, '=');
      
      if (eq) {
	      *eq++ = EOS;
	      opt->valStr = strdup(eq);
      }
      else {
	      opt->valStr = NULL;
      }

      if (strncmp(arg, "--no-", 5) == 0) {
        opt->option = strdup(&arg[5]);
        flags |= inverted;
      }
      else {
      	opt->option = strdup(&arg[2]);
      }
      opt->optType = long_switch;
      opt->flags = flags;

      options.push_back(opt);
    }
    else if (argStr[0] == '-') {
      int len = strlen(argStr);
      char tmp[2];
      
      //printf(" short\n");

      tmp[1] = EOS;

      for (int j=1; j<len; j++) {
        tmp[0] = argv[i][j];

        opt = new option_t();

        opt->option = strdup(tmp);
        opt->valStr = NULL;
        opt->optType = short_switch;
        opt->flags = 0;

        options.push_back(opt);
      }
    }
    else {
      //printf(" argument\n");

      opt = new option_t();

      opt->option = strdup(argStr);
      opt->valStr = NULL;
      opt->optType = argument;
      opt->flags = 0;

      options.push_back(opt);
    }

    i++;
  }
}

const char *Args::getFlagStr(option_t *opt) {
  static char flags[MAXSTR];
  char typeCh = '?';
  char iFlag = '-';

  switch (opt->optType) {
  case short_switch:
    typeCh = 'S';
    break;

  case long_switch:
    typeCh = 'L';
    break;

  case argument:
    typeCh = 'A';
    break;
  }

  if (opt->flags & inverted) {
    iFlag = 'i';
  }

  snprintf(flags, MAXSTR-1, "%c%c", typeCh, iFlag);

  return flags;
}

void Args::dump() {
  printf("Args:\n");

  for (int i=0; i<options.size(); i++) {
    option_t *opt = options.at(i);

    printf("  %2d: %s '%s'", i, getFlagStr(opt), opt->option);
    if (opt->valStr != NULL) {
      printf(" = '%s'", opt->valStr);
    }
    printf("\n");
  }
}

void Args::pairShortArg(char argCh, bool required) {
  option_t *opt;
  int optInd = argIndex(argCh);

  //printf("Short arg '%c' %s have an argument\n", argCh, required?"must":"may");
  //printf("  index=%d\n", optInd);

  if (optInd == -1) {
    return;
  }

  opt = options.at(optInd);
  if (opt->valStr != NULL) {
    // already has one
    return;
  }

  for (int i=optInd+1; i<options.size(); i++) {
    option_t *arg = options.at(i);

    //printf("  does '%s' belong to it?\n", arg->option);

    if (arg->optType == long_switch) {
      if (required) {
	      throw CommandLineException();
      }

      return;
    }

    if (arg->optType == argument) {
      // We're in business
      opt->valStr = arg->option;
      options.erase(options.begin()+i);

      return;
    }
  }
}

void Args::pairShortArgs(const char *shortArgStr) {
  const char *cp = shortArgStr;

  while (*cp) {
    bool required;

    if (cp[1] == ':') {
      required = (cp[2] != ':');

      pairShortArg(*cp, required);

      if (!required) {
	      cp++;
      }
      cp++;
    }
    
    cp++;
  }
}

bool Args::hasArg(const char *arg, int argType) {
  return (argIndex(arg, argType) != -1);
}

int Args::argIndex(const char *arg, int argType) {
  option_t *opt;

  //printf("argIndex('%s', %d)\n", arg, argType);

  for (int i=0; i<options.size(); i++) {
    opt = options.at(i);

    if ((opt->optType == argType) && (strcmp(opt->option, arg) == 0)) {
      return i;
    }
  }

  return -1;
}

bool Args::hasArg(const char *arg) {
  return (argIndex(arg) != -1);
}

int Args::argIndex(const char *arg) {
  int optInd = argIndex(arg, long_switch);

  if (optInd == -1) {
    optInd = argIndex(arg, short_switch);
  }

  return optInd;
}

bool Args::hasArg(const char shortArg) {
  return (argIndex(shortArg) != -1);
}

int Args::argIndex(const char shortArg) {
  char longArg[2];

  longArg[0] = shortArg;
  longArg[1] = EOS;

  return argIndex(longArg, short_switch);
}

void Args::associateShortArgs(parseopt_t *opts) {
  parseopt_t *opt = opts;

  while (opt->optStr != NULL) {
    if (opt->shortOpt) {
      pairShortArg(opt->shortOpt, opt->flags & requires_argument);
    }

    opt++;
  }
}

void Args::processArgs(parseopt_t *opts) {
  parseopt_t *opt = opts;

  while (opt->optStr != NULL) {
    int optInd = argIndex(opt->optStr);

    if (optInd == -1) {
      optInd = argIndex(opt->shortOpt);
    }

    //printf("Arg '%s' -> %d\n", opt->optStr, optInd);
    if (optInd != -1) {
      option_t *arg = options.at(optInd);

      // Do we treat the argument as a string, nunber or flag/boolean?
      if (opt->flags & numeric_argument) {
        long *dest = (long *)opt->target;
        //printf("Numeric\n");

        if (dest != NULL) {
          *dest = Utils::parseAddress(arg->valStr);
        }
      }
      else if (opt->flags & boolean_argument) {
        bool res = false;
        bool *dest = (bool *)opt->target;

        //printf("Boolean\n");
        if (arg->valStr == NULL) {
          res = (arg->flags & inverted)?false:true;
        }
        else {
          if ( (strcasecmp(arg->valStr, "yes") == 0) || 
              (strcasecmp(arg->valStr, "true") == 0) ||
              (strcasecmp(arg->valStr, "1") == 0) ) {
            res = true;
          }

          if (arg->flags & inverted) {
	          res = !res;
	        }
	      }

	      if (dest != NULL) {
	        *dest= res;
	      }
      }
      else {
        //printf("String '%s'\n", arg->valStr);
        if (opt->target != NULL) {
          char *cp = (char *)opt->target;
          strcpy(cp, arg->valStr);
        }
      }
    }

    opt++;
  }
}

void Args::parseArgs(parseopt_t *mandatory, parseopt_t *optional, bool fussy) {
  parseopt_t *opt = mandatory;

  // Deal with any short args that must have an argument
  associateShortArgs(mandatory);

  // Now check all mandatory options are present and correct
  opt = mandatory;
  while (opt->optStr != NULL) {
    int optInd = argIndex(opt->optStr);

    //printf("checking for mandatory arg '%s'\n", opt->optStr);

    if (optInd == -1) {
      //printf("..not found - how about short form '%c'?\n", opt->shortOpt);

      // long form not present, maybe the short one is
      optInd = argIndex(opt->shortOpt);

      if (optInd == -1) {
	      throw CommandLineException();
      }
    }

    //printf("..ok\n");

    opt++;
  }

  // All mandatory args are present

  processArgs(mandatory);
  processArgs(optional);
}

int Args::getArgC() {
  return options.size();
}

int Args::getArgC(int argType) {
  int numArgs = options.size();
  int res = 0;

  for (int i=0; i<numArgs; i++) {
    if (options.at(i)->optType == argType) {
      res++;
    }
  }

  return res;
}

option_t *Args::getArg(int argNum) {
  if ((argNum >= 0) && (argNum < options.size())) {
    return options.at(argNum);
  }

  return NULL;
}

option_t *Args::getArg(int argNum, int argType) {
  int numArgs = options.size();
  int hits = -1;

  printf("getArg(%d, %d)\n");
  dump();

  for (int i=0; i<numArgs; i++) {
    if (options.at(i)->optType == argType) {
      hits++;

      if (hits == argNum) {
        printf("Found at %d\n", i);
        return options.at(i);
      }
    }
  }

  return NULL;
}
