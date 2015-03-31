#pragma once

#include <stdint.h>

#define MEM_BYTES 4096
#define PROGRAM_START_ADDRESS 0x200
#define MAX_PROGRAM_SIZE (MEM_BYTES - PROGRAM_START_ADDRESS)
#define FONTSET_START_ADDRESS 0x50
#define BYTES_PER_LETTER 5
#define SCR_W 64
#define SCR_H 32
#define CYCLES_PER_SECOND 500
#define TICKS_PER_CYCLE ((uint32_t)(1000.0/CYCLES_PER_SECOND))

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