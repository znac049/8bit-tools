#include "conv.h"

MemoryCell::MemoryCell() {
  set(0);
  setType(0);
}

MemoryCell::MemoryCell(int val) {
  set(val);
  setType(0);
}

void MemoryCell::set(int val) {
  value = val;
}

int MemoryCell::get() {
  return value;
}

void MemoryCell::setType(int typ) {
  type = typ;
}

int MemoryCell::getType() {
  return type;
}
