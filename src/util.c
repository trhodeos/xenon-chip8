#include "util.h"

#include <stdio.h>

void memread(byte* dest, const char* file) {
  byte cur;
  FILE *fp;
  int i;

  if (!file) {
    printf("%s: Cannot open null file!\n",__FILE__);
    return;
  }
  
  fp = fopen(file, "rb");
  if (!fp) {
    printf("%s: Problem opening file %s.\n", __FILE__, file);
    return;
  }

  i = 0;
  cur = 0;
  do {
    cur = fgetc(fp);
    dest[i] = cur;
    i++;
  } while (cur != EOF);
  
  fclose(fp);
  fp = 0;
}
