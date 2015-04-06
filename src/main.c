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
static char ROM[512];
static const char* DEFAULT_ROM = "PONG";


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

void set_ROM(char const * const new_rom)
{
	fprintf(stderr, "ROM CHANGED\n");
	if (strlen(ROM) < 512)
	{
		strcpy(ROM, new_rom);
	}
	else
	{
		fprintf(stderr, "ROM file name length exceeded maximum of 512\nLoading %s instead", DEFAULT_ROM);
		strcpy(ROM, DEFAULT_ROM);
	}
}

// Emscripten specific functions
#ifdef __EMSCRIPTEN__
void change_ROM_emscripten(char const * const new_rom)
{
	emscripten_pause_main_loop();
	
	// Reset CPU state and load in new ROM
	cpu_initialize(&cpu);
	set_ROM(new_rom); // sets ROM and checks for errors
	cpu_loadROM(&cpu, ROM); // S* Use return value later for error checking

	emscripten_resume_main_loop();
}
#endif // __EMSCRIPTEN__

int main(int argc, char* argv[])
{
	cpu_initialize(&cpu);

	if (io_initIOModule(&io) == 1)
	{
		fprintf(stderr, "Initializing IO Module Failed\n");
		return 1;
	}

	quit = false;

	// Select and load Startup ROM
	if (argc >= 2)
	{
		strcpy(ROM, argv[1]);
	}
	else
	{
		set_ROM(DEFAULT_ROM);
	}

	if (cpu_loadROM(&cpu, ROM) == 1)
	{
		fprintf(stderr, "Loading Rom Failed\n");
		return 1;
	}

	io_updateWindowTitle(ROM, &io);

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