#include <stdio.h>
#include "chip8.h"
#include "c8_util.h"
#include "c8_actions.h"

int main(int argc, char* argv[])
{
	printf("\n\n\nChip8r\n\n\n\n");

	Chip8 c;

	c8_initialize(&c);
	c8_print_mem(&c, 0, 200);





	return 0;
}