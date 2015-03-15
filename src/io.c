#include "io.h"
#include "CPU.h"

void io_initIOModule(IO_Module * const io)
{
	SDL_Init(SDL_INIT_VIDEO);
	io->window = NULL;
	io->renderer = NULL;
	io->rect = NULL;

	SDL_CreateWindowAndRenderer(640, 320, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &(io->window), &(io->renderer));

	io->rect = (SDL_Rect *)malloc(sizeof(SDL_Rect));
	io->rect->w = 10;
	io->rect->h = 10;
	io->rect->x = 0;
	io->rect->y = 0;
}

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
		io->rect->y = i * 10;
		for (int j = 0; j < SCR_W; ++j)
		{
			io->rect->x = j * 10;
			if (screen[(i * SCR_W) + j])
			{
				SDL_RenderFillRect(io->renderer, io->rect);
			}
		}
	}
	SDL_RenderPresent(io->renderer);
}

uint8_t io_waitForKey(void)
{

}

void io_updateKeys(CPU * const cpu)
{

}
