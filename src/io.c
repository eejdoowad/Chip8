#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "io.h"
#include "CPU.h"

void io_initIOModule(IO_Module * const io)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_CreateWindowAndRenderer(640, 320, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &(io->window), &(io->renderer));
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
	SDL_RenderSetLogicalSize(io->renderer, 64, 32);

	io->e = (SDL_Event *)malloc(sizeof(SDL_Event));

	for (int i = 0; i < 512; ++i)
	{
		io->title[i] = '\0';
	}
}

void io_destroyIOModule(IO_Module * const io)
{
	free(io->e);
	SDL_DestroyRenderer(io->renderer);
	SDL_DestroyWindow(io->window);
	SDL_Quit();
}

void io_updateScreen(IO_Module const * const io, uint8_t * screen)
{
	SDL_SetRenderDrawColor(io->renderer, 0, 0, 0, 255);
	SDL_RenderClear(io->renderer);
	SDL_SetRenderDrawColor(io->renderer, 0, 255, 0, 255);
	for (int i = 0; i < SCR_H; ++i)
	{
		for (int j = 0; j < SCR_W; ++j)
		{
			if (screen[(i * SCR_W) + j])
			{
				SDL_RenderDrawPoint(io->renderer, j, i);
			}
		}
	}
	SDL_RenderPresent(io->renderer);
}

uint8_t io_waitForKey(void)
{

}

void io_updateKeys(CPU * const cpu, IO_Module * const io, bool * const quit)
{
	while (SDL_PollEvent(io->e))
	{
		switch (io->e->type)
		{
			case SDL_QUIT:
				*quit = true;
				break;
			case SDL_WINDOWEVENT:
				break;
		}
	}
}

void io_updateWindowTitle(char const * const ROM, IO_Module * const io)
{
#ifdef _MSC_VER
	sprintf_s(io->title, sizeof(io->title), "Chip 8 Emulator: Running %s at %d FPS", ROM, FPS);
#else
	snprintf(io->title, sizeof(io->title), "Chip 8 Emulator: Running %s at %d FPS", ROM, FPS);
#endif

	SDL_SetWindowTitle(io->window, io->title);
}