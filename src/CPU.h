#pragma once

#include <stdint.h>

#define MEM_BYTES 4096
#define START_ADDRESS 0x200
#define SCR_W 64
#define SCR_H 32
#define FPS 60
#define TICKS_PER_FRAME (1000/FPS)

typedef struct
{
	uint16_t opcode;
	uint8_t mem[MEM_BYTES];
	uint16_t V[16];
	uint16_t PC;
	uint16_t I;
	uint8_t SP;
	uint16_t stack[16];
	uint8_t screen[SCR_W * SCR_H];
	uint8_t delay_timer;
	uint8_t sound_timer;
	uint8_t keys[16];
	uint8_t draw;
} CPU;