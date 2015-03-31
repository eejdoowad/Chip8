#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instructions.h"
#include "io.h"

/////////////////////////////////////////////////////////////////////////
// Interpreting instructions:
// nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
// n or nibble - A 4-bit value, the lowest 4 bits of the instruction
// x - A 4-bit value, the lower 4 bits of the high byte of the instruction
// y - A 4-bit value, the upper 4 bits of the low byte of the instruction
// kk or byte - An 8-bit value, the lowest 8 bits of the instruction
/////////////////////////////////////////////////////////////////////////


static uint16_t op_nnn(const uint16_t opcode)
{
	return opcode & 0x0FFF;
}

static uint8_t op_x(const uint16_t opcode)
{
	return (opcode & 0x0F00) >> 8;
}

static uint8_t op_y(const uint16_t opcode)
{
	return (opcode & 0x00F0) >> 4;
}

static uint8_t op_kk(const uint16_t opcode)
{
	return opcode & 0x00FF;
}

// do nothing instruction
void i_null(CPU * const cpu)
{
	
}

// 0nnn - SYS addr
// Jump to a machine code routine at nnn.
//* This instruction is only used on the old computers on which Chip-8 was originally implemented. It is ignored by modern interpreters.
void i_0nnn(CPU * const cpu)
{
	const uint16_t nnn = op_nnn(cpu->opcode);

	cpu->PC = nnn;
}

// 00E0 - CLS
// Clear the display.
void i_00E0(CPU * const cpu)
{
	memset(cpu->screen, 0, sizeof(cpu->screen));
	cpu->draw = 1;
	cpu->PC += 2;
}


// 00EE - RET
// Return from a subroutine.
// The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
// My note: Add 2 to PC to prevent infinite CALL-RET loop
void i_00EE(CPU * const cpu)
{
	cpu->PC = cpu->stack[cpu->SP];
	cpu->SP -= 1;
	cpu->PC += 2;
}


// 1nnn - JP addr
// Jump to location nnn.
// The interpreter sets the program counter to nnn.
void i_1nnn(CPU * const cpu)
{
	const uint16_t nnn = op_nnn(cpu->opcode);

	cpu->PC = nnn;
}


// 2nnn - CALL addr
// Call subroutine at nnn.
// The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
void i_2nnn(CPU * const cpu)
{
	const uint16_t nnn = op_nnn(cpu->opcode);

	cpu->SP += 1;
	cpu->stack[cpu->SP] = cpu->PC;
	cpu->PC = nnn;
}

// 3xkk - SE Vx, byte
// Skip next instruction if Vx = kk.
// The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
void i_3xkk(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);
	const uint8_t kk = op_kk(cpu->opcode);

	cpu->PC += ((cpu->V[x] == kk) ? 4 : 2);
}

// 4xkk - SNE Vx, byte
// Skip next instruction if Vx != kk.
// The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
void i_4xkk(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);
	const uint8_t kk = op_kk(cpu->opcode);

	cpu->PC += ((cpu->V[x] != kk) ? 4 : 2);
}

// 5xy0 - SE Vx, Vy
// Skip next instruction if Vx = Vy.
// The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
void i_5xy0(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);
	const uint8_t y = op_y(cpu->opcode);

	cpu->PC += ((cpu->V[x] == cpu->V[y]) ? 4 : 2);
}

// 6xkk - LD Vx, byte
// Set Vx = kk.
// The interpreter puts the value kk into register Vx.
void i_6xkk(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);
	const uint8_t kk = op_kk(cpu->opcode);

	cpu->V[x] = kk;
	cpu->PC += 2;
}

// 7xkk - ADD Vx, byte
// Set Vx = Vx + kk.
// Adds the value kk to the value of register Vx, then stores the result in Vx. 
void i_7xkk(CPU * const cpu)
{//
	const uint8_t x = op_x(cpu->opcode);
	const uint8_t kk = op_kk(cpu->opcode);

	cpu->V[x] = (cpu->V[x] + kk) & 0xFF; // S* Not sure about masking!!! but it helps merlin
	cpu->PC += 2;
}

// 8xy0 - LD Vx, Vy
// Set Vx = Vy.
// Stores the value of register Vy in register Vx.
void i_8xy0(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);
	const uint8_t y = op_y(cpu->opcode);

	cpu->V[x] = cpu->V[y];
	cpu->PC += 2;
}

// 8xy1 - OR Vx, Vy
// Set Vx = Vx OR Vy.
// Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. 
// A bitwise OR compares the corrseponding bits from two values, and if either bit is 1
// then the same bit in the result is also 1. Otherwise, it is 0. 
void i_8xy1(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);
	const uint8_t y = op_y(cpu->opcode);

	cpu->V[x] |= cpu->V[y];
	cpu->PC += 2;
}

// 8xy2 - AND Vx, Vy
// Set Vx = Vx AND Vy.
// Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. 
// A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, 
// then the same bit in the result is also 1. Otherwise, it is 0. 
void i_8xy2(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);
	const uint8_t y = op_y(cpu->opcode);

	cpu->V[x] &= cpu->V[y];
	cpu->PC += 2;
}

// 8xy3 - XOR Vx, Vy
// Set Vx = Vx XOR Vy.
// Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. 
// An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, 
// then the corresponding bit in the result is set to 1. Otherwise, it is 0. 
void i_8xy3(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);
	const uint8_t y = op_y(cpu->opcode);

	cpu->V[x] ^= cpu->V[y];
	cpu->PC += 2;
}

// 8xy4 - ADD Vx, Vy
// Set Vx = Vx + Vy, set VF = carry.
// The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) 
// VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
void i_8xy4(CPU * const cpu)
{// S*
	const uint8_t x = op_x(cpu->opcode);
	const uint8_t y = op_y(cpu->opcode);
	
	cpu->V[0xF] = (cpu->V[x] > (0xFF - cpu->V[y]));
	cpu->V[x] = (cpu->V[x] + cpu->V[y]) & 0x00FF;
	cpu->PC += 2;
}

// 8xy5 - SUB Vx, Vy
// Set Vx = Vx - Vy, set VF = NOT borrow.
// If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
// My Note: must have ment Vx >= Vy
void i_8xy5(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);
	const uint8_t y = op_y(cpu->opcode);

	cpu->V[0xF] = (cpu->V[x] >= cpu->V[y]);
	cpu->V[x] = (cpu->V[x] - cpu->V[y]);
	cpu->PC += 2;
}

// 8xy6 - SHR Vx {, Vy}
// Set Vx = Vx SHR 1.
// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
// Vy is unused.
void i_8xy6(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);

	cpu->V[0xF] = cpu->V[x] & 0x0001;
	cpu->V[x] >>= 1;
	cpu->PC += 2;
}

// 8xy7 - SUBN Vx, Vy
// Set Vx = Vy - Vx, set VF = NOT borrow.
// If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
// My Note: SO WHICH IS IT!!! > or >= ???
void i_8xy7(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);
	const uint8_t y = op_y(cpu->opcode);

	cpu->V[0xF] = (cpu->V[x] <= cpu->V[y]);
	cpu->V[x] = cpu->V[y] - cpu->V[x];
	cpu->PC += 2;
}

// 8xyE - SHL Vx {, Vy}
// Set Vx = Vx SHL 1.
// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
void i_8xyE(CPU * const cpu)
{ // S*
	const uint8_t x = op_x(cpu->opcode);

	cpu->V[0xF] = (cpu->V[x] & 0x8000);
	cpu->V[x] <<= 1;;
	cpu->PC += 2;
}

// 9xy0 - SNE Vx, Vy
// Skip next instruction if Vx != Vy.
// The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
void i_9xy0(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);
	const uint8_t y = op_y(cpu->opcode);

	cpu->PC += ((cpu->V[x] != cpu->V[y]) ? 4 : 2);
}

// Annn - LD I, addr
// Set I = nnn.
// The value of register I is set to nnn.
void i_Annn(CPU * const cpu)
{
	const uint16_t nnn = op_nnn(cpu->opcode);

	cpu->I = nnn;
	cpu->PC += 2;
}

// Bnnn - JP V0, addr
// Jump to location nnn + V0.
// The program counter is set to nnn plus the value of V0.
void i_Bnnn(CPU * const cpu)
{
	const uint16_t nnn = op_nnn(cpu->opcode);

	cpu->PC = cpu->V[0x0] + nnn;
}

// Cxkk - RND Vx, byte
// Set Vx = random byte AND kk.
// The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. 
// The results are stored in Vx. See instruction 8xy2 for more information on AND.
void i_Cxkk(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);
	const uint8_t kk = op_kk(cpu->opcode);

	cpu->V[x] = (uint16_t) ((rand() % 256) & kk); 
	cpu->PC += 2;
}

// Dxyn - DRW Vx, Vy, nibble
// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
// The interpreter reads n bytes from memory, starting at the address stored in I. 
// These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
// Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0.
//* If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen.
// See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
void i_Dxyn(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);
	const uint8_t y = op_y(cpu->opcode);
	const uint8_t n = cpu->opcode & 0x000F;
	const uint16_t Vx = cpu->V[x];
	const uint16_t Vy = cpu->V[y];

	for (uint8_t sprite_row = 0; sprite_row < n; ++sprite_row)
	{
		uint8_t byte_to_write = cpu->mem[cpu->I + sprite_row];

		for (uint8_t bit = 0; bit < 8; ++bit)
		{
			if (byte_to_write & (0x80 >> bit))
			{
				const uint8_t x_coordinate = (Vx + bit) % SCR_W;
				const uint8_t y_coordinate = (Vy + sprite_row) % SCR_H;
				const uint16_t pixel_index = ((uint16_t) y_coordinate * SCR_W) + x_coordinate;
				if (cpu->screen[pixel_index] == 0x01)
				{
					cpu->V[0xF] = 1;
				}
				cpu->screen[pixel_index] ^= 0x01;
				
			}
		}
	}
	cpu->draw = 1;
	cpu->PC += 2;
}

// Ex9E - SKP Vx
// Skip next instruction if key with the value of Vx is pressed.
// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
void i_Ex9E(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);

	cpu->PC += (cpu->keys[cpu->V[x]] ? 4 : 2); // S* try switching around 4 and 2, is keydown 0 or 1?
}

// ExA1 - SKNP Vx
// Skip next instruction if key with the value of Vx is not pressed.
// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
void i_ExA1(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);

	cpu->PC += (cpu->keys[cpu->V[x]] ? 2 : 4); // S* try switching around 4 and 2, is keydown 0 or 1?
}

// Fx07 - LD Vx, DT
// Set Vx = delay timer value.
// The value of DT is placed into Vx.
void i_Fx07(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);

	cpu->V[x] = cpu->delay_timer;
	cpu->PC += 2;
}

// Fx0A - LD Vx, K
// Wait for a key press, store the value of the key in Vx.
// All execution stops until a key is pressed, then the value of that key is stored in Vx.
void i_Fx0A(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);

	cpu->V[x] = io_waitForKey();
	cpu->PC += 2;
}

// Fx15 - LD DT, Vx
// Set delay timer = Vx.
// DT is set equal to the value of Vx.
void i_Fx15(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);

	cpu->delay_timer = (uint8_t) cpu->V[x];
	cpu->PC += 2;
}

// Fx18 - LD ST, Vx
// Set sound timer = Vx.
// ST is set equal to the value of Vx.
void i_Fx18(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);

	cpu->sound_timer = (uint8_t) cpu->V[x];
	cpu->PC += 2;
}


// Fx1E - ADD I, Vx
// Set I = I + Vx.
// The values of I and Vx are added, and the results are stored in I.
void i_Fx1E(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);

	cpu->I += cpu->V[x];
	cpu->PC += 2;
}

// Fx29 - LD F, Vx
// Set I = location of sprite for digit Vx.
// The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx. 
// See section 2.4, Display, for more information on the Chip-8 hexadecimal font.
void i_Fx29(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);

	cpu->I = FONTSET_START_ADDRESS + (BYTES_PER_LETTER * cpu->V[x]);
	cpu->PC += 2;
}

// Fx33 - LD B, Vx
// Store BCD representation of Vx in memory locations I, I+1, and I+2.
// The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at 
// location in I, the tens digit at location I+1, and the ones digit at location I+2.
void i_Fx33(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);
	const uint16_t Vx = cpu->V[x];

	cpu->mem[cpu->I] = Vx / 100;	// hundreds
	cpu->mem[cpu->I + 1] = (Vx / 10) % 10;	// tens
	cpu->mem[cpu->I + 2] = Vx % 10; // ones
	cpu->PC += 2;
}

// Fx55 - LD [I], Vx
// Store registers V0 through Vx in memory starting at location I.
// The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
void i_Fx55(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);

	for (int i = 0; i <= x; ++i)
	{
		cpu->mem[cpu->I + (2 * i)] = (uint8_t) (cpu->V[i] >> 8);
		cpu->mem[cpu->I + (2 * i) + 1] = (uint8_t) (cpu->V[i]);
	}
	cpu->PC += 2;
}

// Fx65 - LD Vx, [I]
// Read registers V0 through Vx from memory starting at location I.
// The interpreter reads values from memory starting at location I into registers V0 through Vx.
void i_Fx65(CPU * const cpu)
{
	const uint8_t x = op_x(cpu->opcode);

	for (int i = 0; i <= x; ++i)
	{
		cpu->V[i] = cpu->mem[cpu->I + i];
	}
	cpu->PC += 2;
}





