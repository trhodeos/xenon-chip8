#include "chip8.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
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
  memread(&memory[FONT_START_ADDR], "data/fontset.bin");
}

void load_rom(const char* file) {
  memread(&memory[PROGRAM_START_ADDR], file);
}

int poll_input() {
  // TODO
  return 0;
}

void emulate_cycle() {

  ushort x, y, height, pixel, xline, yline;
  int reg_x, reg_y, address, value, i, temp;

  // opcodes are 2 bytes, not 1
  ushort opcode = memory[pc] << 8 | memory[pc+1];

  reg_x = (opcode & 0x0F00) >> 8;
  reg_y = (opcode & 0x00F0) >> 4;

  value = (opcode & 0x00FF);
  address = (opcode & 0x0FFF);
  // Decode
  switch (opcode & 0xF000) {
    case 0x0000:
      switch (opcode & 0x0F00) {
        case 0x0000:
          switch (opcode & 0x00FF) {
            case 0x00E0: // 0x00E0: Clear screen
              memset(gfx, 0, sizeof(gfx));
              pc += 2;
              break;
            case 0x00EE: // 0x00EE: Return from subroutine
              pc = stack[sp];
              sp--;
              break;
          }
          break;
        default: // 0x0NNN: Sys call.. deprecated
          /* empty */
          printf("Warning: syscall functionality deprecated\n");
          break;
      }
      break;
    case 0x1000: // 0x1NNN: Jump to 0x0NNN
      pc = address;
      break;
    case 0x2000: // 0x2NNN: Subroutine call to 0x0NNN
      sp++;
      stack[sp] = pc;
      pc = address;
      break;
    case 0x3000: // 0x3XKK: Skip next instruction if equal
      if (V[reg_x] == value) {
        pc += 2;
      }
      pc += 2;
      break;
    case 0x4000: // 0x4XKK: Skip next instruction if not equal
      if (V[reg_x] != value) {
        pc += 2;
      }
      pc += 2;

      break;
    case 0x5000: // 0x5XY0: Skip next instruction if V[x] == V[y]
      if (V[reg_x] == V[reg_y]) {
        pc += 2;
      }
      pc += 2;
      break;
    case 0x6000: // 0x6XKK: Load
      V[reg_x] = value;
      pc += 2;
      break;
    case 0x7000: // 0x7XKK: Add
      V[reg_x] += value;
      pc += 2;
      break;
    case 0x8000:
      switch (opcode & 0x000F) {
        case 0x0000: // 0x8XY0: Load 
          V[reg_x] = V[reg_y];
          pc += 2;
          break;
        case 0x0001: // 0x8XY1: Or
          V[reg_x] |= V[reg_y];
          pc += 2;
          break;
        case 0x0002: // 0x8XY2: And
          V[reg_x] &= V[reg_y];
          pc += 2;
          break;
        case 0x0003: // 0x8XY3: Xor
          V[reg_x] ^= V[reg_y];
          pc += 2;
          break;
        case 0x0004: // 0x8XY4: Add
          if ((int)(V[reg_x] + V[reg_y]) > 255) {
            V[0xF] = 1;
          } else {
            V[0xF] = 0;
          }
          V[reg_x] += V[reg_y];
          pc += 2;
          break;
        case 0x0005: // 0x8XY5: Subtract
          if (V[reg_x] > V[reg_y]) {
            V[0xF] = 1;
          } else {
            V[0xF] = 0;
          }
          V[reg_x] -= V[reg_y];
          pc += 2;
          break;
        case 0x0006: // 0x8XY6: Shift right
          V[0xF] = (opcode & 0x0001);
          V[reg_x] = V[reg_x] >> 1;
          pc += 2;
          break;
        case 0x0007: // 0x8XY7: Subtract from y
          if (V[reg_x] < V[reg_y]) {
            V[0xF] = 1;
          } else {
            V[0xF] = 0;
          }
          V[reg_x] = V[reg_y] - V[reg_x];
          pc += 2;
          break;
        case 0x000E: // 0x8XYE: Shift left
          V[0xF] = (opcode & 0x8000) >> 15;
          V[reg_x] = V[reg_x] << 1;
          pc += 2;
          break;
      }
      break;
    case 0x9000: // 0x9XY0: Skip not equal
      if (V[reg_x] != V[reg_y]) {
        pc += 2;
      }
      pc += 2;
      break;
    case 0xA000: // ANNN: Sets I to the address 0xNNN
      I = address;
      pc += 2;
      break;
    case 0xB000: // 0xBNNN: Jump to nnn + V0
      pc = address + V[0];
      break;
    case 0xC000: // 0xCXKK: Random
      temp = rand() % 256;
      V[reg_x] = temp & value;
      pc += 2;
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

            gfx[x + xline + ((y + yline) * 64)] ^= 0x1;
          }
        }
      }
      draw_flag = 1;
      pc += 2;
      break;
    case 0xE000:
      switch (opcode & 0x00FF) {
        case 0x009E: // 0xEX9E: Skip if key pressed
          if (key[V[reg_x]] != 0) {
            pc += 2;
          }
          pc += 2;
          break;
        case 0x00A1: // 0xEXA1: Skip if key not pressed
          if (key[V[reg_x]] == 0) {
            pc += 2;
          }
          pc += 2;
          break;
      }
      break;
    case 0xF000:
      switch (opcode & 0x00FF) {
        case 0x0007: // 0xFX07: Load delay timer into v[x]
          V[reg_x] = delay_timer;
          pc += 2;
          break;
        case 0x000A:
          while ((value = poll_input()) < 0) {
            /* do nothing */
          }
          V[reg_x] = value;
          pc += 2;
          break;
        case 0x0015: // 0xFX15: Load into delay timer
          delay_timer = V[reg_x];
          pc += 2;
          break;
        case 0x0018: // 0xFX18: Load into sound timer
          sound_timer = V[reg_x];
          pc += 2;
          break;
        case 0x001E: // 0xFX1E: Add to I
          I += V[reg_x];
          pc += 2;
          break;
        case 0x0029: // 0xFX29: Load digit address
          I = FONT_START_ADDR + V[reg_x] * FONT_DIGIT_WIDTH;
          pc += 2;
          break;
        case 0x0033: // 0xFX31: Store BCD in I, I+1, I+2
          memory[I] = V[reg_x] / 100;
          memory[I+1] = (V[reg_x] % 100) / 10;
          memory[I+2] = (V[reg_x] % 10);
          pc += 2;
          break;
        case 0x0055: // 0xFX55: Store registers
          for (i = 0; i <= reg_x; i++) {
            memory[I + i] = V[i];
          }
          pc += 2;
          break;
        case 0x0065: // 0xFX65: Load registers
          for (i = 0; i <= reg_x; i++) {
            V[i] = memory[I + i];
          }
          pc += 2;
          break;
      }
      break;
    default:
      printf("%s: Opcode 0x%x not yet implemented\n", __FILE__, opcode);
      break;
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
