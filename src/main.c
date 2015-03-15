#include <stdio.h>
#include <SDL2/SDL.h>
#include "CPU.h"
#include "io.h"
#include "c8_actions.h"
#include "instructions.h"


int main(int argc, char* argv[])
{

	if (argc == 1)
	{
		fprintf(stderr, "No ROM Selected. Exiting\n");
		return 1;
	}
	else if (argc > 2)
	{
		fprintf(stderr, "Too many arguments. There should only be two. Exiting.\n");
		return 1;
	}

	CPU cpu;
	cpu_initialize(&cpu);

	IO_Module io;
	io_initIOModule(&io);

	cpu_loadROM(&cpu, argv[1]);

	for (int cycle = 0; ; ++cycle)
	{
		cpu_emulateCycle(&cpu);
		if (cpu.draw)
		{
			io_updateScreen(&io, cpu.screen);
			cpu.draw = 0;
		}

		io_updateKeys(&cpu);	
	}


	//cpu_print_regs(&cpu);

	//cpu.opcode = 0x1235;
	//cpu_i_jumpImmediate(&cpu);
	//cpu_print_regs(&cpu);

	//// Dxyn - DRW Vx, Vy, nibble
	//// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
	//cpu.opcode = 0xD015;
	//cpu.V[0] = 0;
	//cpu.V[1] = 0;
	//cpu.I = 0x50;
	//cpu_i_draw(&cpu);
	//cpu_print_screen(&cpu);

	io_destroyIOModule(&io);
	return 0;
}