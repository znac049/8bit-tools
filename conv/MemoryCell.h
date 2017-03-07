#ifndef _MEMORY_CELL_H_
#define _MEMORY_CELL_H_

class MemoryCell {
 private:
  int value;
  int type;

 protected:
 public:

 private:
 protected:
 public:
  MemoryCell();
  MemoryCell(int val);

  void set(int val);
  int get();

  void setType(int typ);
  int getType();
};

#endif
