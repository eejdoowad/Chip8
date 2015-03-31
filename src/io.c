#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "io.h"
#include "CPU.h"

// returns 1 on failure, 0 on success
int io_initIOModule(IO_Module * const io)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	}
	if (SDL_CreateWindowAndRenderer(640, 320, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &(io->window), &(io->renderer)) < 0)
	{
		fprintf(stderr, "SDL_CreateWindowAndRenderer failed: %s\n", SDL_GetError());
		return 1;
	}
	SDL_RenderSetLogicalSize(io->renderer, 64, 32);
	for (int i = 0; i < 512; ++i)
	{
		io->title[i] = '\0';
	}
	return 0;
}

static  SDL_Keycode key_mappings[16] =
{
	SDLK_1, SDLK_2, SDLK_3, SDLK_4,
	SDLK_q, SDLK_w, SDLK_e, SDLK_r,
	SDLK_a, SDLK_s, SDLK_d, SDLK_f,
	SDLK_z, SDLK_x, SDLK_c, SDLK_v
};

void io_destroyIOModule(IO_Module * const io)
{
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
	static SDL_Event e;
	for (;;)
	{
		SDL_WaitEvent(&e);
		for (uint8_t i = 0; i < 16; ++i)
		{
			if (key_mappings[i] == e.key.keysym.sym)
			{
				return i;
			}
		}
	}
}

void io_updateKeys(CPU * const cpu, IO_Module * const io, bool * const quit)
{
	static SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_QUIT:
				*quit = true;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						*quit = true;
						break;
				}
				for (int i = 0; i < 16; ++i)
				{
					if (key_mappings[i] == e.key.keysym.sym)
					{
						cpu->keys[i] = 1;
					}
				}
				break;
			case SDL_KEYUP:
				for (int i = 0; i < 16; ++i)
				{
					if (key_mappings[i] == e.key.keysym.sym)
					{
						cpu->keys[i] = 0;
					}
				}
				break;
		}
	}
}

void io_updateWindowTitle(char const * const ROM, IO_Module * const io)
{
#ifdef _MSC_VER
	sprintf_s(io->title, sizeof(io->title), "Chip 8 Emulator: Running %s at %d Cycles Per Second", ROM, CYCLES_PER_SECOND);
#else
	snprintf(io->title, sizeof(io->title), "Chip 8 Emulator: Running %s at %d Cycles Per Second", ROM, CYCLES_PER_SECOND);
#endif
	SDL_SetWindowTitle(io->window, io->title);
}
