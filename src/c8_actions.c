#include "c8_actions.h"


// initializes registers and memory
void c8_initialize(Chip8 * const c8)
{
	// Reset Registers
	c8->PC = START_ADDRESS;
	c8->opcode = 0x00;
	c8->I = 0x00;
	c8->SP = 0x0;

	for (int i = 0; i < 16; ++i)
	{
		c8->stack[i] = 0x00;
		c8->V[i] = 0x00;
	}

	c8_zeroMemory(c8);
	c8_clearDisplay(c8);
	c8_loadFontSet(c8);

	// Reset timers
	// TODO
}

void c8_loadFontSet(Chip8 * const c8) // loads font set into first 80 memory addresses
{
	uint8_t chip8_fontset[80] =
	{ 
	  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	  0x20, 0x60, 0x20, 0x20, 0x70, // 1
	  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	for (int i = 0; i < 80; ++i)
	{
		c8->mem[i] = chip8_fontset[i];
	}
}

void c8_zeroMemory(Chip8 * const c8) // initializes all of memory to zero
{
	for (int i = 0; i < MEM_BYTES; ++i)
	{
		c8->mem[i] = 0x00;
	}
}


void c8_clearDisplay(Chip8 * const c8) // clears the display
{
		// uint8_t	screen[SCR_W * SCR_H];
	for (int i = 0; i < SCR_H; ++i)
	{
		for (int j = 0; j < SCR_W; ++j)
		{
			c8->screen[(i * SCR_H) + j] = 0;
			printf("Zeroing %d, %d", i, j);
		}
		putchar('\n');
	}

}

// performs fetch, decode, execute, and timer update
void c8_emulateCycle(Chip8 * const c8)
{
	// Fetch Opcode
	c8->opcode = ((uint16_t)c8->mem[c8->PC] << 8) | (c8->mem[c8->PC + 1]);

	//* Should decode and execute be coupled into a single function? load next pc at end of execution
	//* Decode and Execute Opcode
	c8_decodeAndExecute(c8);

	//* Update timers
}

void c8_loadGame(Chip8 * const c8, char const * const game_file) //loads the game into memory
{
	uint16_t buffer_size = 0;
	uint8_t buffer[MEM_BYTES - START_ADDRESS];
	//* open game_file using fopen()

	//	load game file
		for (int i = 0; i < buffer_size; ++i)
		{
			c8->mem[i + START_ADDRESS] = buffer[i];
		}
}



void c8_decodeAndExecute(Chip8 * const c8)
{

}