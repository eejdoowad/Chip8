#include <stdio.h>
#include <time.h>
#include <string.h>
#include "CPU.h"
#include "c8_util.h"
#include "c8_actions.h"
#include "instructions.h"
#include "input.h"

void temp(char const * const str)
{
	printf("%s", str);
}

int main(int argc, char* argv[])
{
	char ROM[256] = "";

	if (argc == 1)
	{
		printf("No ROM Selected. Exiting\n");
		return 1;
	}
	else if (argc == 2)
	{
		strcpy(ROM, "../roms/");
		strcpy(ROM + 8,argv[1]);
		printf("%s\n", ROM);
	}
	else
	{
		printf("Too many arguments. There should only be two. Exiting.\n");
		return 1;
	}
	temp(ROM);

	srand(time(NULL));

	CPU cpu;

	cpu_initialize(&cpu);
	cpu_loadROM(&cpu, ROM);
	//cpu_print_everything(&c, 0x200, 0x280);


	for (int i = 0; ; ++i)
	{
		cpu_emulateCycle(&cpu);

		if (cpu.draw)
		{
			draw_to_screen(&cpu);			
			cpu.draw = 0;
		}

		update_keys(&cpu);



		cpu_print_screen(&cpu);
		printf("End of cycle: %d", i);
		// char c;
		// while ((c = getchar()) != '\n')
		// {
		// 	if (c == 'm')
		// 	{
		// 		cpu_print_mem(&cpu, 0x200, 0x280);
		// 	}
		// 	else if (c == 'r')
		// 	{
		// 		cpu_print_regs(&cpu);
		// 	}
		// 	else if (c == 'q')
		// 	{
		// 		exit(1);
		// 	}
		// }
		if (i%100 == 0)
			getchar();
		
	}


	cpu_print_regs(&cpu);

	cpu.opcode = 0x1235;
	cpu_i_jumpImmediate(&cpu);
	cpu_print_regs(&cpu);

	// Dxyn - DRW Vx, Vy, nibble
	// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
	cpu.opcode = 0xD015;
	cpu.V[0] = 0;
	cpu.V[1] = 0;
	cpu.I = 0x50;
	cpu_i_draw(&cpu);
	cpu_print_screen(&cpu);


	//cpu_run(&c);

	return 0;
}