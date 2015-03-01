#include <stdio.h>
#include "chip8.h"
#include "c8_util.h"

int main(int argc, const char** argv)
{
	printf("\n\n\nChip8r\n\n\n\n");

	struct chip8 c;

	c8_print_mem(c.mem, 0, 200);


	return 0;
}