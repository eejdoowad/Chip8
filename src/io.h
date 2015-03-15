#pragma once

#include <SDL2/SDL.h>
#include <stdint.h>
#include "CPU.h"

typedef struct
{
	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Event * e;
	char title[512]; // sort of arbitrary size. not sure how big it needs to be
} IO_Module;


void io_initIOModule(IO_Module * const io);
void io_destroyIOModule(IO_Module * const io);
void io_updateScreen(IO_Module const * const io, uint8_t * screen);

uint8_t io_waitForKey(void);

void io_updateKeys(CPU * const cpu, IO_Module * const io, int * const quit);

void io_updateWindowTitle(char const * const ROM, IO_Module * const io);