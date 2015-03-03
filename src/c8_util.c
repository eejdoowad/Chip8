#include "c8_util.h"

void cpu_print_regs(CPU const * const cpu)
{
	printf("op 0x%04x     PC 0x%04x     I 0x%04x\nSP 0x%02x       delay 0x%02x    sound 0x%02x\n\n", cpu->opcode, cpu->PC, cpu->I, cpu->SP, cpu->delay_timer, cpu->sound_timer);
	printf("#     V        Stack    Keys\n");
	for (int i = 0; i < 16; i++)
	{
		printf("%1x     %04x     %04x     %04x\n", i, cpu->V[i], cpu->stack[i], cpu->key[i]);
	}
}

void cpu_print_screen(CPU const * const cpu)
{
	printf("    0         1         2         3         4         5         6   \n");
	printf("    0123456789012345678901234567890123456789012345678901234567890123\n\n");
	for (int i = 0; i < SCR_H; i++)
	{
		printf("%02d  ", i);
		for (int j = 0; j < SCR_W; j++)
		{
			putchar(cpu->screen[i * SCR_W + j] ? '@' : ' ');
		}
		printf("  %02d\n", i);
	}
	printf("\n");
	printf("    0         1         2         3         4         5         6   \n");
	printf("    0123456789012345678901234567890123456789012345678901234567890123\n\n");
}


void cpu_print_mem(CPU const * const cpu, uint16_t first, uint16_t last)
{
	if ((first < 0) || (last > 4096) || first > last)
	{
		printf("Failure: Memory Selection out of range.");
		exit(1);
	}

	uint8_t const * const mem = cpu->mem;
	//print heading row header
	printf("------"); //10 spaces
	for (uint16_t i = 0; i < 0xF; ++i)
		printf("%X--", i);
	printf("F-");
	//print starting address
	printf("\n0x%02X:", first>>4);
	//print padding starting XXXX's
	for (uint16_t i = 0; i < (first & 0x000F); ++i)
	{
		printf("   ");
	}
	//print first row memory after padding
	for (uint16_t i = first & 0x000F; i <= 0xF; ++i)
	{
		printf(" %02X", mem[(first & 0xFFF0) + i]); //check validity later on
	}
	//print rows first + 1 to last - 1 //again check validity later
	for (uint16_t i = (first & 0xFFF0) + 0x10; i < (last & 0xFFF0); i += 0x10)
	{
		printf("\n0x%02X:", i>>4);
		for (int j = 0; j <= 0xF; ++j)
		{
			printf(" %02X", mem[i + j]);
		}
		if ((i & 0x00F0) == 0x00F0) //print heading row every 8 lines /////////////////////heres a glitch. too tired.
		{
			printf("\n------");
			for (uint16_t i = 0; i < 0xF; ++i)
				printf("%X--", i);
			printf("F-");
		}
	//for (uint16_t i = 0; i <= 0xF; ++i) printf("%X --- ", i);
	}
	//print last valid row //definitely are much easier ways to index through
	printf("\n0x%02X:", last>>4);
	for (uint16_t i = 0; i <= (last & 0x000F); ++i)
	{
		printf(" %02X", mem[(last & 0xFFF0) + i]);
	}
	//print closing padding XXXX's
	for (uint16_t i = last + 1; i <= (last | 0x000F); ++i)
	{
		printf("   ");
	}
	printf("\n\n");
}

void cpu_print_everything(CPU const * const cpu, uint16_t first, uint16_t last)
{
	cpu_print_regs(cpu);
	cpu_print_screen(cpu);
	cpu_print_mem(cpu, first, last);
}