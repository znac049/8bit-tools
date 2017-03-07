#ifndef _UTILS_H_
#define _UTILS_H_

class Utils {
 private:
 public:
  static char *truncStr(char *s, char c);
  static bool isHexChar(char c);
  static int hexVal(char c);
  static long hex2int(char *str);
  static long parseAddress(char *str);

  static bool isFile(const char *path);

  static void ltrim(char *s);
  static void rtrim(char *s);
  static void trim(char *s);

  static char printableChar(char ch);

  static void abortf(const char *fmt, ...);
};

#endif
