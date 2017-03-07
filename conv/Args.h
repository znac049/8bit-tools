#ifndef _ARGS_H_
#define _ARGS_H_

#include <vector>

typedef struct ParseOpt {
  const char *optStr;
  int flags;
  void *target;
  char shortOpt;
} parseopt_t;

typedef struct Option {
  char *option;
  char *valStr;
  int optType;
  int flags;
} option_t;

class Args {
 private:
  std::vector<option_t *> options;
  char *longName;
  char *shortName;

 protected:
 public:
  enum argType {short_switch, long_switch, argument};
  enum flags {inverted=1};
  enum optionFlags {requires_argument=1, optional_argument=2, boolean_argument=4, numeric_argument=8};

 private:
  const char *getFlagStr(option_t *opt);
  int argIndex(const char *arg, int argType);
  int argIndex(const char *arg);
  int argIndex(const char shortArg);
  void pairShortArg(char argCh, bool required);
  void pairShortArgs(const char *shortArgStr);
  void associateShortArgs(parseopt_t *opts);
  void processArgs(parseopt_t *opts);

 protected:
 public:
  Args(int argc, char **argv);

  void dump();

  void parseArgs(parseopt_t *mandatory, parseopt_t *optional, bool fussy);

  int getArgC();
  int getArgC(int argType);

  option_t *getArg(int argNum);
  option_t *getArg(int argNum, int argType);

  bool hasArg(const char *arg, int argType);
  bool hasArg(const char *arg);
  bool hasArg(const char shortArg);
};

#endif
