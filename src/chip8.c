#include "chip8.h"
#include "util.h"

#include <console/console.h>

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

void emulate_cycle() {

  ushort x, y, height, pixel, xline, yline;

  // Fetch
  // opcodes are 2 bytes, not 1
  ushort opcode = memory[pc] << 8 | memory[pc+1];

  // Decode
  switch(opcode & 0xF000) {
    case 0xA000: // ANNN: Sets I to the address 0xNNN
      I = opcode & 0x0FFF;
      pc +=2;
      break;
    case 0xD000: // DXYN: Draws *I to (X,Y) with height N
      x = V[(opcode & 0x0F00) >> 8];
      y = V[(opcode & 0x0F00) >> 4];
      height = opcode & 0x000F;

      V[0xF] = 0;
      for (yline = 0; yline < height; yline++) {
        pixel = memory[I + yline];
        for (xline = 0; xline < 8; xline++) {
          if ((pixel & (0x80 >> xline)) != 0) {
            if (gfx[(x + xline + ((y + yline) * 64))] == 1) {
              V[0xF] = 1;
            }

            gfx[x + xline + ((y + yline) * 64)] ^= 1;
          }
        }
      }
      draw_flag = 1;
      pc += 2;
      break;

    default:
      break;
      //      printf("%s: Opcode 0x%x not yet implemented\n", __FILE__, opcode);
  }

  if (delay_timer > 0)
    delay_timer--;
  
  if (sound_timer > 0) {
    if (sound_timer == 1) {
      //      printf("BEEP\n");
    }
    sound_timer--;
  }
}
