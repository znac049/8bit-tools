#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <exception>

using namespace std;

class RangeException: public std::exception {
  virtual const char* what() const throw() {
    return "Range exception";
  }
};

class ConversionException: public std::exception {
  virtual const char* what() const throw() {
    return "Conversion exception";
  }
};

class CommandLineException: public std::exception {
  virtual const char* what() const throw() {
    return "Bad command line exception";
  }
};

#endif
