#include "chip8.h"
#include "util.h"

// for memset
#include <string.h>

void chip8_init() {

  memset(memory, 0, sizeof(memory));
  memset(gfx, 0, sizeof(gfx));
  memset(V, 0, sizeof(V));

  I = 0x000;
  pc = PROGRAM_START_ADDR; // always the first instruction location

  memset(stack, 0, sizeof(stack));
  sp = 0;

  memset(key, 0, sizeof(key));

  delay_timer = 0;
  sound_timer = 0;

  // read in font set
  memread(&memory[FONT_START_ADDR], "fontset.bin");
}

void load_rom(const char* file) {
  memread(&memory[PROGRAM_START_ADDR], file);
}
