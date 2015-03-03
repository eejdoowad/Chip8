#include "c8_actions.h"
#include <stdio.h>
#include <stdlib.h>

// initializes registers and memory
void cpu_initialize(CPU * const cpu)
{
	// Reset Registers
	cpu->PC = START_ADDRESS;
	cpu->opcode = 0x00;
	cpu->I = 0x00;
	cpu->SP = 0x0;

	for (int i = 0; i < 16; ++i)
	{
		cpu->stack[i] = 0x00;
		cpu->V[i] = 0x00;
	}

	cpu_zeroMemory(cpu);
	cpu_clearDisplay(cpu);
	cpu_loadFontSet(cpu);

	// Reset timers
	// TODO
}

void cpu_loadFontSet(CPU * const cpu) // loads font set into first 80 memory addresses
{
	uint8_t CPU_fontset[80] =
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
		cpu->mem[0x50 + i] = CPU_fontset[i];
	}
}

void cpu_zeroMemory(CPU * const cpu) // initializes all of memory to zero
{
	for (int i = 0; i < MEM_BYTES; ++i)
	{
		cpu->mem[i] = 0x00;
	}
}


void cpu_clearDisplay(CPU * const cpu) // clears the display
{
		// uint8_t	screen[SCR_W * SCR_H];
	for (int i = 0; i < SCR_H; ++i)
	{
		for (int j = 0; j < SCR_W; ++j)
		{
			cpu->screen[(i * SCR_W) + j] = 0;
		}
	}

}

void cpu_run(CPU * const cpu)
{
	for (;;)
	{

	}
}

// performs fetch, decode, execute, and timer update
void cpu_emulateCycle(CPU * const cpu)
{
	// Fetch Opcode
	cpu->opcode = ((uint16_t)cpu->mem[cpu->PC] << 8) | (cpu->mem[cpu->PC + 1]);

	//* Should decode and execute be coupled into a single function? load next pc at end of execution
	//* Decode and Execute Opcode
	cpu_decodeAndExecute(cpu);

	//* Update timers
}

void cpu_loadROM(CPU * const cpu, char const * const game_file) //loads the game into memory
{
	FILE * fp;
	if ((fp = fopen(game_file, "r")) == NULL)
	{
		fprintf(stderr, "Opening ROM failed");
		exit(1);
	}

	int numread = fread(cpu->mem + START_ADDRESS, 1, MEM_BYTES - START_ADDRESS, fp);
	printf("\nBytes read from ROM: %d\n", numread);
	fclose(fp);
}



void cpu_decodeAndExecute(CPU * const cpu)
{

}