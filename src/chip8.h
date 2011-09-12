#ifndef CHIP8_H
#define CHIP8_H

// -----------------------------------------------
// memory map
// -----------------------------------------------

#define INTERPRETER_START_ADDR 0x000
#define FONT_START_ADDR 0x050
#define FONT_END_ADDR 0x0A0
#define INTERPRETER_END_ADDR 0x1ff

// -----------------------------------------------
// types
// -----------------------------------------------

// general
typedef unsigned short ushort;
typedef unsigned char uchar;

// chip-8 specific
typedef ushort opcode;
typedef uchar byte;

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

#endif // CHIP8_H
