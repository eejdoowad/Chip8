#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "c8_actions.h"
#include "c8_util.h"
#include "instructions.h"

static void cpu_loadFontSet(CPU * const cpu);
static void cpu_zeroMemory(CPU * const cpu);
static void (*instruction_table[16])(CPU * const cpu);
static void i_0xxx(CPU * const cpu);
static void i_8xxx(CPU * const cpu);
static void i_Exxx(CPU * const cpu);
static void i_Fxxx(CPU * const cpu);

// initializes registers and memory
void cpu_initialize(CPU * const cpu)
{
	// Reset Registers
	cpu->PC = PROGRAM_START_ADDRESS;
	cpu->opcode = 0x00;
	cpu->I = 0x00;
	cpu->SP = 0x0;

	memset(cpu->stack, 0, sizeof(cpu->stack));
	memset(cpu->V, 0, sizeof(cpu->V));

	cpu_zeroMemory(cpu);
	memset(cpu->screen, 0, sizeof(cpu->screen));
	cpu_loadFontSet(cpu);

	cpu->delay_timer = 0;
	cpu->sound_timer = 0;

	cpu->draw = 0x0;

	srand((unsigned)time(NULL));
}

// loads font set into first 80 memory addresses
void cpu_loadFontSet(CPU * const cpu) 
{
	static const uint8_t CPU_fontset[80] =
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

	memcpy(cpu->mem + FONTSET_START_ADDRESS, CPU_fontset, sizeof(CPU_fontset));
}

void cpu_zeroMemory(CPU * const cpu) // initializes all of memory to zero
{
	for (int i = 0; i < MEM_BYTES; ++i)
	{
		cpu->mem[i] = 0x00;
	}
}


// performs fetch, decode, execute, and timer update
void cpu_emulateCycle(CPU * const cpu)
{
	// Fetch Opcode
	cpu->opcode = ((uint16_t)cpu->mem[cpu->PC] << 8) | (cpu->mem[cpu->PC + 1]);

	
	cpu_print_current_instruction(cpu);


	// deocode and execute
	instruction_table[(cpu->opcode & 0xF000) >> 12](cpu);

	// Update timers
	if (cpu->sound_timer > 0)
	{
		cpu->sound_timer -= 1;
	}
	if (cpu->delay_timer > 0)
	{
		cpu->delay_timer -= 1;
	}
}

//loads the game into memory
void cpu_loadROM(CPU * const cpu, char const * const game_file) 
{
	FILE * fp;
	if ((fp = fopen(game_file, "rb")) == NULL)
	{
		fprintf(stderr, "Opening ROM  \"%s\" failed\n", game_file);
		fflush(stderr);
		exit(1);
	}

	int numread = fread(cpu->mem + PROGRAM_START_ADDRESS, 1, MEM_BYTES - PROGRAM_START_ADDRESS, fp);
	printf("\nBytes read from ROM: %d\n", numread);
	fflush(stdout);
	fclose(fp);
}

// array of function pointer where the functions correspond to the
// instruction to be executed given the high order opcode nibble
static void (*instruction_table[16])(CPU * const cpu) = 
{
	i_0xxx,		// 0
	i_1nnn, 	// 1
	i_2nnn, 	// 2
	i_3xkk, 	// 3
	i_4xkk,		// 4
	i_5xy0, 	// 5
	i_6xkk, 	// 6
	i_7xkk, 	// 7 
	i_8xxx, 	// 8
	i_9xy0, 	// 9
	i_Annn, 	// A
	i_Bnnn, 	// B
	i_Cxkk, 	// C
	i_Dxyn, 	// D
	i_Exxx, 	// E
	i_Fxxx 		// F
};

// handles instructions with high-order nibble 8
void i_8xxx(CPU * const cpu)
{
	switch (cpu->opcode & 0x000F)
	{
		case 0:
			i_8xy0(cpu);
			break;
		case 1:
			i_8xy1(cpu);
			break;
		case 2:
			i_8xy1(cpu);
			break;
		case 3:
			i_8xy3(cpu);
			break;
		case 4:
			i_8xy4(cpu);
			break;
		case 5:
			i_8xy5(cpu);
			break;
		case 6:
			i_8xy6(cpu);
			break;
		case 7:
			i_8xy7(cpu);
			break;
		case 0xE:
			i_8xyE(cpu);
			break;
	}
}

// handles instructions with high-order nibble 0
void i_0xxx(CPU * const cpu)
{
	if (cpu->opcode == 0x00E0)
	{
		i_00E0(cpu);
	}
	else if (cpu->opcode == 0x00EE)
	{
		i_00EE(cpu);
	}
	else
	{
		i_0nnn(cpu);
	}
}

// handles instructions with high-order nibble E
void i_Exxx(CPU * const cpu)
{
	if ((cpu->opcode & 0xF0FF) == 0xE09E)
	{
		i_Ex9E(cpu);
	}
	else
	{
		i_ExA1(cpu);
	}
}

// handles instructions with high-order nibble F
void i_Fxxx(CPU * const cpu)
{
	switch (cpu->opcode & 0x00FF)
	{
		case 0x07 :
			i_Fx07(cpu); 
			break;
		case 0x0A :
			i_Fx0A(cpu);
			break;
		case 0x15 :
			i_Fx15(cpu);
			break;
		case 0x18 :
			i_Fx18(cpu);
			break;
		case 0x1E :
			i_Fx1E(cpu);
			break;
		case 0x29 :
			i_Fx29(cpu);
			break;
		case 0x33 :
			i_Fx33(cpu);
			break;
		case 0x55 :
			i_Fx55(cpu);
			break;
		case 0x65 :
			i_Fx65(cpu);
			break;
	}
}