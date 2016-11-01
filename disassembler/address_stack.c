#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "bda.h"

unsigned short AddressStack[K64];
int InsertInd=0;

int get_address_stack_size() {
  return InsertInd;
}

unsigned short get_address_stack_at(int addr) {
  return AddressStack[addr];
}

int is_entry_point(unsigned short addr) {
  int i;

  for (i = 0; i < InsertInd; i++) {
    if (AddressStack[i] == addr) {
      return YES;
    }
  }

  return NO;
}

void stack_address(unsigned short addr) {
  int i;

  if ((addr < 0x6000) || (addr > 0x7fff)) {
    return;
  }

  if (get_memory_type(addr) == MEM_CODE) {
    /* already been there, so no need to revisit */
    return;
  }

  for (i = 0; i < InsertInd; i++) {
    if (AddressStack[i] == addr) {
      return;
    }
  }

  /* printf("Stacking address $%04x for later processing\n", addr); */

  AddressStack[InsertInd++] = addr;
}

