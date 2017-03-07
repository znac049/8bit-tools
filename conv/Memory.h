#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <vector>

class Memory {
 private:
  std::vector<MemoryCell *> memory;
  long memorySize;
  long highestAddress;
  long lowestAddress;
  char padChar;

 protected:
 public:
  enum MemoryType {UNKNOWN='.', BYTE='b'};

 private:
  void assertAddressValid(long addr);

 protected:
 public:
  Memory(long mSize);

  long readFile(const char *file_name, long addr);

  bool isValidAddress(long addr);

  int getByte(long addr);
  void setByte(long addr, int val);

  int setType(long addr, int type, int count);
  void setPadChar(char pad);

  long getLowestAddress();
  long getHighestAddress();

  void info();
  void dump();
};

#endif
