#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
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

	unsigned const startTime = SDL_GetTicks();
	unsigned cycleStartTime;
	unsigned cycleElapsedTime;

	bool quit = false;
	for (int cycle = 0; !quit; ++cycle)
	{
		cycleStartTime = SDL_GetTicks();

		cpu_emulateCycle(&cpu);
		if (cpu.draw)
		{
			io_updateScreen(&io, cpu.screen);
			cpu.draw = 0;
		}

		io_updateKeys(&cpu, &io, &quit);

		cycleElapsedTime = SDL_GetTicks() - cycleStartTime;
		if (cycleElapsedTime < TICKS_PER_FRAME)
		{
			SDL_Delay(TICKS_PER_FRAME - cycleElapsedTime);
		}

		io_updateWindowTitle(argv[1], &io);
	}

	io_destroyIOModule(&io);
	return 0;
}