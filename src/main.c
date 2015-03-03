#include <stdio.h>
#include "CPU.h"
#include "c8_util.h"
#include "c8_actions.h"
#include "instructions.h"

int main(int argc, char* argv[])
{
	printf("\n\n\nCPUr\n\n\n\n");

	CPU c;

	cpu_initialize(&c);
	cpu_loadROM(&c, "roms/BRIX");
	//cpu_print_everything(&c, 0x200, 0x280);


	cpu_print_regs(&c);

	c.opcode = 0x1235;
	c8_i_jumpImmediate(&c);
	cpu_print_regs(&c);

	// Dxyn - DRW Vx, Vy, nibble
	// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
	c.opcode = 0xD015;
	c.V[0] = 10;
	c.V[1] = 10;
	c.I = 0x55;
	c8_i_draw(&c);


	// c.opcode = 0x0005;
	// c.I = 0x50;
	// c8_i_draw(&c);
	cpu_print_screen(&c);

	//cpu_run(&c);

	return 0;
}