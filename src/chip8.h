#ifndef CHIP8_H
#define CHIP8_H

#include "types.h"

// -----------------------------------------------
// memory map
// -----------------------------------------------

#define INTERPRETER_START_ADDR 0x000
#define FONT_START_ADDR 0x050
#define FONT_END_ADDR 0x0A0
#define INTERPRETER_END_ADDR 0x1ff
#define PROGRAM_START_ADDR 0x200


// -----------------------------------------------
// other constants
// -----------------------------------------------

#define FONT_DIGIT_WIDTH 0x0005
// -----------------------------------------------
// system
// -----------------------------------------------

// main memory
byte memory[4096];

// graphics
byte gfx[64 * 32];

// 15 8-bit general purpose registers
// 16th register is used for carry-flag
byte V[16];

// index register
ushort I;

// program counter
ushort pc;

// stack information
ushort stack[16];
ushort sp;

// key-press info
uchar key[16];

// timers
uchar delay_timer;
uchar sound_timer;

// -----------------------------------------------
// implementation specific
// -----------------------------------------------

byte draw_flag;

// -----------------------------------------------
// function declarations
// -----------------------------------------------
void chip8_init();
void emulate_cycle();
void load_rom(const char* file);


#endif // CHIP8_H
