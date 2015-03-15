#include <stdio.h>
#include <time.h>
#include <string.h>
#include <SDL/SDL.h>
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
	// SDL
	const int WIDTH = 640;
	const int HEIGHT = 320;

	SDL_Init(SDL_INIT_VIDEO);


	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &window, &renderer);
	
	SDL_Rect rect;
	rect.h = 64;
	rect.w = 32;
	rect.x = 0;
	rect.y = 0;
	// END SDL

	char ROM[256] = "";

	if (argc == 1)
	{
		printf("No ROM Selected. Exiting\n");
		return 1;
	}
	else if (argc == 2)
	{
		strcpy(ROM, argv[1]);
		printf("%s\n", ROM);
	}
	else
	{
		printf("Too many arguments. There should only be two. Exiting.\n");
		return 1;
	}
	temp(ROM);

	srand((unsigned) time(NULL));

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
			// SDL //
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			for (int i = 0; i < 64; ++i)
			{
				rect.y = i * 10;
				for (int j = 0; j < 32; ++j)
				{
					rect.x = j * 10;
					if (cpu.screen[(i * SCR_W) + j])
					{
						SDL_RenderFillRect(renderer, &rect);
					}
				}
			}
			SDL_RenderPresent(renderer);

			SDL_Delay(5);
			// END SDL //


		}

		update_keys(&cpu);
		
		
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



	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}