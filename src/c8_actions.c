#include "c8_actions.h"
#include "instructions.h"
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
	cpu->draw = 0x0;

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
		cpu->opcode = ((uint16_t)cpu->mem[cpu->PC] << 8) | (cpu->mem[cpu->PC + 1]);
	}
}

// performs fetch, decode, execute, and timer update
void cpu_emulateCycle(CPU * const cpu)
{
	// Fetch Opcode
	cpu->opcode = ((uint16_t)cpu->mem[cpu->PC] << 8) | (cpu->mem[cpu->PC + 1]);

	// deocode and execute
	cpu_decodeAndExecute(cpu);

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

void cpu_loadROM(CPU * const cpu, char const * const game_file) //loads the game into memory
{
	FILE * fp;
	if ((fp = fopen(game_file, "r")) == NULL)
	{
		fprintf(stderr, "Opening ROM failed");
		system("dir");
		exit(1);
	}

	int numread = fread(cpu->mem + START_ADDRESS, 1, MEM_BYTES - START_ADDRESS, fp);
	printf("\nBytes read from ROM: %d\n", numread);
	fclose(fp);
}

void cpu_decodeAndExecute(CPU * const cpu)
{

	cpu_instruction_table[(cpu->opcode & 0xF000) >> 12](cpu);
}

void (*cpu_instruction_table[16])(CPU * const cpu) = 
{
	cpu_0xxx_instruction, 			// 0, high order nibble
	cpu_i_jumpImmediate, 			// 1
	cpu_i_callSubroutine, 			// 2
	cpu_i_skipEqualImmediate, 		// 3
	cpu_i_skipNotEqualImmediate,		// 4
	cpu_i_skipEqualRegister, 		// 5
	cpu_i_loadImmediate, 			// 6
	cpu_i_addImmediate, 				// 7 
	cpu_8xxx_instruction, 			// 8
	cpu_i_skipNotEqualRegister, 		// 9
	cpu_i_loadToI, 					// A
	cpu_i_jumpV0Offset, 				// B
	cpu_i_loadRandom, 				// C
	cpu_i_draw, 						// D
	cpu_Exxx_instruction, 			// E
	cpu_Fxxx_instruction 			// F
};

void cpu_8xxx_instruction(CPU * const cpu)
{
	//* switch (cpu->opcode & 0x000F)
}

void cpu_0xxx_instruction(CPU * const cpu) // handles instructions with high-order nibble 0
{
	if (cpu->opcode == 0x00E0)
	{
		cpu_i_clearDisplay(cpu);
	}
	else if (cpu->opcode == 0x00EE)
	{
		cpu_i_subroutineReturn(cpu);
	}
	else
	{
		cpu_i_jumpRoutine(cpu);
	}
}

void cpu_Exxx_instruction(CPU * const cpu) // handles instructions with high-order nibble E
{
	if ((cpu->opcode & 0xF0FF) == 0xE09E)
	{
		cpu_i_skipButtonPressed(cpu);
	}
	else
	{
		cpu_i_skipNotPressed(cpu);
	}
}

void cpu_Fxxx_instruction(CPU * const cpu) // handles instructions with high-order nibble F
{
	switch (cpu->opcode & 0x00FF)
	{
		case 0x07 :
			cpu_i_loadDelayTimer(cpu); 			break;
		case 0x0A :
			cpu_i_waitForThenStoreButton(cpu); 	break;
		case 0x15 :
			cpu_i_loadToDelayTimer(cpu); 		break;
		case 0x18 :
			cpu_i_loadToSoundTimer(cpu); 		break;
		case 0x1E :
			cpu_i_addToI(cpu); 					break;
		case 0x29 :
			cpu_i_loadFontToI(cpu); 			break;
		case 0x33 :
			cpu_i_loadBCD(cpu); 				break;
		case 0x55 :
			cpu_i_storeVRegisters(cpu); 		break;
		case 0x65 :
			cpu_i_loadVRegisters(cpu); 			break;
		default :
			fprintf(stderr, "Instruction fell through Fxxx swith table");
	}
}