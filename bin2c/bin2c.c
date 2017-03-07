#include <stdio.h>

int main(int argc, char **argv)
{
  int b;
  int first = 1;
  int count = 0;

  printf("const byte ROMImage[4096] PROGMEM = {");

  while ((b = getchar()) != EOF) {
    if (!first) {
      printf(", ");
    }
    else {
      first = 0;
    }

    if ((count & 0x0f) == 0) {
      printf("\n\t\t");
    }

    printf("0x%02x", b);
    count++;
  }
  printf("\n};\n\n");
}
