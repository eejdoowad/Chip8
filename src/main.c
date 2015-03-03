#include <stdio.h>
#include "CPU.h"
#include "c8_util.h"
#include "c8_actions.h"

int main(int argc, char* argv[])
{
	printf("\n\n\nCPUr\n\n\n\n");

	CPU c;

	cpu_initialize(&c);
	cpu_loadROM(&c, "roms/BRIX");
	cpu_print_everything(&c, 0x200, 0x280);

	cpu_run(&c);

	return 0;
}