#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "CPU.h"
#include "io.h"
#include "c8_actions.h"
#include "c8_util.h"
#include "instructions.h"

static CPU cpu;
static IO_Module io;
static bool quit;

void one_cycle(void)
{
	cpu_emulateCycle(&cpu);
	if (cpu.draw)
	{
		io_updateScreen(&io, cpu.screen);
		cpu.draw = 0;
	}
	io_updateKeys(&cpu, &io, &quit);
}

int main(int argc, char* argv[])
{
	cpu_initialize(&cpu);

	if (io_initIOModule(&io) == 1)
	{
		fprintf(stderr, "Initializing IO Module Failed\n");
		return 1;
	}

	quit = false;
	
	char * ROM_TO_LOAD = "PONG";
	if (argc >= 2)
	{
		ROM_TO_LOAD = argv[1];
	}

	if (cpu_loadROM(&cpu, ROM_TO_LOAD) == 1)
	{
		fprintf(stderr, "Loading Rom Failed\n");
		return 1;
	}

	io_updateWindowTitle(ROM_TO_LOAD, &io);

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(one_cycle, CYCLES_PER_SECOND, 1);
#else // __EMSCRIPTEN__
	uint32_t cycleStartTime;
	uint32_t cycleEndTime;
	uint32_t cycleElapsedTime;

	for (int cycle = 0; !quit; ++cycle)
	{
		cycleStartTime = SDL_GetTicks();

		one_cycle();

		cycleEndTime = SDL_GetTicks();
		cycleElapsedTime =  cycleEndTime - cycleStartTime;
		if (cycleElapsedTime < TICKS_PER_CYCLE)
		{
			SDL_Delay(TICKS_PER_CYCLE - cycleElapsedTime);
		}
	}
#endif // __EMSCRIPTEN__
	io_destroyIOModule(&io);
	return 0;
}