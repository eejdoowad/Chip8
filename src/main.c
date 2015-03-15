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

	int quit = 0;
	for (int cycle = 0; !quit; ++cycle)
	{
		cpu_emulateCycle(&cpu);
		if (cpu.draw)
		{
			io_updateScreen(&io, cpu.screen);
			cpu.draw = 0;
		}

		io_updateKeys(&cpu, &io, &quit);	
	}

	io_destroyIOModule(&io);
	return 0;
}