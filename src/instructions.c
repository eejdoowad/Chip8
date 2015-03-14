// instructions.c

#include "instructions.h"
#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include "input.h"

//* Used for unimplemented instructions
void cpu_cpuNULL(CPU * const cpu)
{
	printf("\n\n************* NULL OPCODE *************\n\n");
}


// nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
// n or nibble - A 4-bit value, the lowest 4 bits of the instruction
// x - A 4-bit value, the lower 4 bits of the high byte of the instruction
// y - A 4-bit value, the upper 4 bits of the low byte of the instruction
// kk or byte - An 8-bit value, the lowest 8 bits of the instruction

// 0nnn - SYS addr
// Jump to a machine code routine at nnn.
//* This instruction is only used on the old computers on which Chip-8 was originally implemented. It is ignored by modern interpreters.
void cpu_i_jumpRoutine(CPU * const cpu)
{
	cpu->PC = cpu->opcode & 0x0FFF;
}

// 00E0 - CLS
// Clear the display.
void cpu_i_clearDisplay(CPU * const cpu)
{
	for (int row = 0; row < SCR_H; ++row)
	{
		for (int col = 0; col < SCR_W; ++col)
		{
			cpu->screen[(row * SCR_H) + col] = 0x0;
		}
	}
	cpu->draw = 1;
	cpu->PC += 2;
}


// 00EE - RET
// Return from a subroutine.
// The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
void cpu_i_subroutineReturn(CPU * const cpu)
{
	cpu->PC = cpu->stack[cpu->SP];
	if (cpu->SP == 0)
	{
		fprintf(stderr, "Stack Pointer zero. Cannot subtract from SP in cpu_i_subroutineReturn."); //* confirm
		exit(1);
	}
	cpu->SP -= 1;
}


// 1nnn - JP addr
// Jump to location nnn.
// The interpreter sets the program counter to nnn.
void cpu_i_jumpImmediate(CPU * const cpu)
{
	cpu->PC = cpu->opcode & 0x0FFF;
}


// 2nnn - CALL addr
// Call subroutine at nnn.
// The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
void cpu_i_callSubroutine(CPU * const cpu)
{
	cpu->SP += 1;
	cpu->stack[cpu->SP] = cpu->PC;
	cpu->PC = cpu->opcode & 0x0FFF;
}

// 3xkk - SE Vx, byte
// Skip next instruction if Vx = kk.
// The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
void cpu_i_skipEqualImmediate(CPU * const cpu)
{
	uint16_t PC_increment = 2;
	if (cpu->V[(cpu->opcode & 0x0F00) >> 8] == (cpu->opcode & 0x00FF))
	{
		PC_increment = 4;
	}
	cpu->PC += PC_increment;
}

// 4xkk - SNE Vx, byte
// Skip next instruction if Vx != kk.
// The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
void cpu_i_skipNotEqualImmediate(CPU * const cpu)
{
	uint16_t PC_increment = 2;
	if (cpu->V[(cpu->opcode & 0x0F00) >> 8] != (cpu->opcode & 0x00FF))
	{
		PC_increment = 4;
	}
	cpu->PC += PC_increment;
}

// 5xy0 - SE Vx, Vy
// Skip next instruction if Vx = Vy.
// The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
void cpu_i_skipEqualRegister(CPU * const cpu)
{
	uint16_t PC_increment = 2;
	if (cpu->V[(cpu->opcode & 0x0F00) >> 8] == cpu->V[(cpu->opcode & 0x00F0) >> 4])
	{
		PC_increment = 4;
	}
	cpu->PC += PC_increment;
}

// 6xkk - LD Vx, byte
// Set Vx = kk.
// The interpreter puts the value kk into register Vx.
void cpu_i_loadImmediate(CPU * const cpu)
{
	cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->opcode & 0x00FF;
	cpu->PC += 2;
}

// 7xkk - ADD Vx, byte
// Set Vx = Vx + kk.
// Adds the value kk to the value of register Vx, then stores the result in Vx. 
void cpu_i_addImmediate(CPU * const cpu)
{
	cpu->V[(cpu->opcode & 0x0F00) >> 8] += (cpu->opcode & 0x00FF);
	cpu->PC += 2;
}

// 8xy0 - LD Vx, Vy
// Set Vx = Vy.
// Stores the value of register Vy in register Vx.
void cpu_i_loadRegister(CPU * const cpu)
{
	cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->V[(cpu->opcode & 0x00F0) >> 4];
	cpu->PC += 2;
}

// 8xy1 - OR Vx, Vy
// Set Vx = Vx OR Vy.
// Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. 
// A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, 
// then the same bit in the result is also 1. Otherwise, it is 0. 
void cpu_i_OR(CPU * const cpu)
{
	cpu->V[(cpu->opcode & 0x0F00) >> 8] |= cpu->V[(cpu->opcode & 0x00F0) >> 4];
	cpu->PC += 2;
}

// 8xy2 - AND Vx, Vy
// Set Vx = Vx AND Vy.
// Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. 
// A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, 
// then the same bit in the result is also 1. Otherwise, it is 0. 
void cpu_i_AND(CPU * const cpu)
{
	cpu->V[(cpu->opcode & 0x0F00) >> 8] &= cpu->V[(cpu->opcode & 0x00F0) >> 4];
	cpu->PC += 2;
}

// 8xy3 - XOR Vx, Vy
// Set Vx = Vx XOR Vy.
// Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. 
// An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, 
// then the corresponding bit in the result is set to 1. Otherwise, it is 0. 
void cpu_i_XOR(CPU * const cpu)
{
	cpu->V[(cpu->opcode & 0x0F00) >> 8] ^= cpu->V[(cpu->opcode & 0x00F0) >> 4];
	cpu->PC += 2;
}

// 8xy4 - ADD Vx, Vy
// Set Vx = Vx + Vy, set VF = carry.
// The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) 
// VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
void cpu_i_addRegister(CPU * const cpu)
{
	if (cpu->V[(cpu->opcode & 0x0F00) >> 8] > (0xFF - cpu->V[(cpu->opcode & 0x00F0) >> 4]))
	{
		cpu->V[0xF] = 1;
	}
	else
	{
		cpu->V[0xF] = 0;
	}
	cpu->V[(cpu->opcode & 0x0F00) >> 8] += cpu->V[(cpu->opcode & 0x00F0) >> 4];
	cpu->PC += 2;
}

// 8xy5 - SUB Vx, Vy
// Set Vx = Vx - Vy, set VF = NOT borrow.
// If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
void cpu_i_subtractRegister(CPU * const cpu)
{
	if (cpu->V[(cpu->opcode & 0x0F00) >> 8] > cpu->V[(cpu->opcode & 0x00F0) >> 4])
	{
		cpu->V[0xF] = 1;
	}
	else
	{
		cpu->V[0xF] = 0;
	}
	cpu->V[(cpu->opcode & 0x0F00) >> 8] -= cpu->V[(cpu->opcode & 0x00F0) >> 4];
	cpu->PC += 2;
}

// 8xy6 - SHR Vx {, Vy}
// Set Vx = Vx SHR 1.
// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
// Vy is unused.
void cpu_i_shiftRight(CPU * const cpu)
{
	if (cpu->V[(cpu->opcode & 0x0F00) >> 8] & 0x0001)
	{
		cpu->V[0xF] = 1;
	}
	else
	{
		cpu->V[0xF] = 0;
	}
	cpu->V[(cpu->opcode & 0x0F00) >> 8] = (cpu->V[(cpu->opcode & 0x0F00) >> 8] >> 1);
	cpu->PC += 2;
}

// 8xy7 - SUBN Vx, Vy
// Set Vx = Vy - Vx, set VF = NOT borrow.
// If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
void cpu_i_subtractRegisterSwitched(CPU * const cpu)
{
	if (cpu->V[(cpu->opcode & 0x0F00) >> 8] <= cpu->V[(cpu->opcode & 0x00F0) >> 4])
	{
		cpu->V[0xF] = 1;
	}
	else
	{
		cpu->V[0xF] = 0;
	}
	cpu->V[(cpu->opcode & 0x0F00) >> 8] = 
		cpu->V[(cpu->opcode & 0x00F0) >> 4] - cpu->V[(cpu->opcode & 0x0F00) >> 8];
	cpu->PC += 2;
}

// 8xyE - SHL Vx {, Vy}
// Set Vx = Vx SHL 1.
// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
void cpu_i_shiftLeft(CPU * const cpu)
{
	if (cpu->V[(cpu->opcode & 0x0F00) >> 8] & 0x8000)
	{
		cpu->V[0xF] = 1;
	}
	else
	{
		cpu->V[0xF] = 0;
	}
	cpu->V[(cpu->opcode & 0x0F00) >> 8] = (cpu->V[(cpu->opcode & 0x0F00) >> 8] << 1);
	cpu->PC += 2;
}

// 9xy0 - SNE Vx, Vy
// Skip next instruction if Vx != Vy.
// The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
void cpu_i_skipNotEqualRegister(CPU * const cpu)
{
	uint16_t PC_increment = 2;
	if (cpu->V[(cpu->opcode & 0x0F00) >> 8] != cpu->V[(cpu->opcode & 0x00F0) >> 4])
	{
		PC_increment = 4;
	}
	cpu->PC += PC_increment;
}

// Annn - LD I, addr
// Set I = nnn.
// The value of register I is set to nnn.
void cpu_i_loadToI(CPU * const cpu)
{
	cpu->I = cpu->opcode & 0x0FFF;
	cpu->PC += 2;
}

// Bnnn - JP V0, addr
// Jump to location nnn + V0.
// The program counter is set to nnn plus the value of V0.
void cpu_i_jumpV0Offset(CPU * const cpu)
{
	cpu->PC = cpu->V[0x0] + (cpu->opcode & 0x0FFF);
}

// Cxkk - RND Vx, byte
// Set Vx = random byte AND kk.
// The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. 
// The results are stored in Vx. See instruction 8xy2 for more information on AND.
// THIS IS SUSPECT. WHY AND???
void cpu_i_loadRandom(CPU * const cpu)
{
	cpu->V[(cpu->opcode & 0x0F00) >> 8] = 
		(uint16_t)(rand() % 256) &  (cpu->opcode & 0x00FF); 
	cpu->PC += 2;
}

// Dxyn - DRW Vx, Vy, nibble
// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
// The interpreter reads n bytes from memory, starting at the address stored in I. 
// These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
// Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0.
//* If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen.
// See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
void cpu_i_draw(CPU * const cpu)
{
	uint16_t VF_status = 0;

	uint8_t height = cpu->opcode & 0x000F;
	uint8_t x = cpu->V[(cpu->opcode & 0x0F00) >> 8];
	uint8_t y = cpu->V[(cpu->opcode & 0x00F0) >> 4];

	for (uint8_t row = 0; row < height; ++row)
	{
		uint8_t byte_to_write = cpu->mem[cpu->I + row];
		for (uint8_t bit = 0; bit < 8; ++bit)
		{
			if ((byte_to_write & (0x80 >> bit)) != 0x00)
			{
				if (cpu->screen[((y + row) * SCR_W) + (x + bit)] == 0x01)
				{
					cpu->V[0xF] = 1;
				}
				cpu->screen[((y + row) * SCR_W) + (x + bit)] ^= 0x01;
			}
		}
	}
	cpu->draw = 1;
	cpu->PC += 2;
}

// Ex9E - SKP Vx
// Skip next instruction if key with the value of Vx is pressed.
// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
void cpu_i_skipButtonPressed(CPU * const cpu)
{
	cpu->PC += (cpu->keys[(cpu->opcode & 0x0F00) >> 8]) ? 4 : 2;
}

// ExA1 - SKNP Vx
// Skip next instruction if key with the value of Vx is not pressed.
// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
void cpu_i_skipNotPressed(CPU * const cpu)
{
	cpu->PC += (cpu->keys[(cpu->opcode & 0x0F00) >> 8]) ? 2 : 4;
}

// Fx07 - LD Vx, DT
// Set Vx = delay timer value.
// The value of DT is placed into Vx.
void cpu_i_loadDelayTimer(CPU * const cpu)
{
	cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->delay_timer;
	cpu->PC += 2;
}

// Fx0A - LD Vx, K
// Wait for a key press, store the value of the key in Vx.
// All execution stops until a key is pressed, then the value of that key is stored in Vx.
void cpu_i_waitForThenStoreButton(CPU * const cpu)
{
	cpu->V[(cpu->opcode & 0x0F00) >> 8] = wait_for_key();
	cpu->PC += 2;
}

// Fx15 - LD DT, Vx
// Set delay timer = Vx.
// DT is set equal to the value of Vx.
void cpu_i_loadToDelayTimer(CPU * const cpu)
{
	cpu->delay_timer = cpu->V[(cpu->opcode & 0x0F00) >> 8];
	cpu->PC += 2;
}

// Fx18 - LD ST, Vx
// Set sound timer = Vx.
// ST is set equal to the value of Vx.
void cpu_i_loadToSoundTimer(CPU * const cpu)
{
	cpu->sound_timer = cpu->V[(cpu->opcode & 0x0F00) >> 8];
	cpu->PC += 2;
}


// Fx1E - ADD I, Vx
// Set I = I + Vx.
// The values of I and Vx are added, and the results are stored in I.
void cpu_i_addToI(CPU * const cpu)
{
	cpu->I += ((cpu->opcode & 0x0F00) >> 8);
	cpu->PC += 2;
}

// Fx29 - LD F, Vx
// Set I = location of sprite for digit Vx.
// The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx. 
// See section 2.4, Display, for more information on the Chip-8 hexadecimal font.
void cpu_i_loadFontToI(CPU * const cpu)
{
	// fonts for HEX digits start at 0x50 and are 5 bytes each
	cpu->I = 0x50 + (5 * ((cpu->opcode & 0x0F00) >> 8));
	cpu->PC += 2;
}

// Fx33 - LD B, Vx
// Store BCD representation of Vx in memory locations I, I+1, and I+2.
// The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at 
// location in I, the tens digit at location I+1, and the ones digit at location I+2.
void cpu_i_loadBCD(CPU * const cpu)
{
	uint16_t Vx = cpu->V[(cpu->opcode & 0x0F00) >> 8];
	cpu->mem[cpu->I] = Vx / 100;	// hundreds
	cpu->mem[cpu->I + 1] = (Vx / 10) % 10;	// tens
	cpu->mem[cpu->I + 2] = Vx % 10; // ones
	cpu->PC += 2;
}

// Fx55 - LD [I], Vx
// Store registers V0 through Vx in memory starting at location I.
// The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
void cpu_i_storeVRegisters(CPU * const cpu)
{
	const int x = (cpu->opcode >> 8) & 0xF;
	for (int i = 0; i <= x; ++i)
	{
		cpu->mem[cpu->I + (2 * i)] = 		cpu->V[i] >> 8;
		cpu->mem[cpu->I + (2 * i) + 1] =	cpu->V[i] & 0x00FF;
	}
	cpu->PC += 2;
}

// Fx65 - LD Vx, [I]
// Read registers V0 through Vx from memory starting at location I.
// The interpreter reads values from memory starting at location I into registers V0 through Vx.
void cpu_i_loadVRegisters(CPU * const cpu)
{
	const int x = (cpu->opcode >> 8) & 0xF;
	for (int i = 0; i <= x; ++i)
	{
		cpu->V[i] = ((uint16_t) (cpu->mem[cpu->I + (2 * i)] << 8)) | cpu->mem[cpu->I + (2 * i) + 1];
	}
	cpu->PC += 2;
}





