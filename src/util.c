#include "util.h"

#include <stdio.h>
#include <unistd.h>

void read_into_mem(byte* dest, const char* file) {
  byte cur;
  FILE *fp = NULL;
  int i;

  if (!dest) {
    printf("%s: Cannot write to null buffer!\n", __FILE__);
    return;
  }

  if (!file) {
    printf("%s: Cannot open null file!\n", __FILE__);
    return;
  }

  fp = fopen(file, "rb");
  if (!fp) {
    printf("%s: Problem opening file %s.\n", __FILE__, file);
    return;
  }

  i = 0;
  cur = 0;
  while (!feof(fp)) {
    cur = fgetc(fp);
    dest[i] = cur;
    i++;
  }
  
  fclose(fp);
  fp = NULL;
}
