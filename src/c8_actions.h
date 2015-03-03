#pragma once

#include "CPU.h"
#include "instructions.h"

void cpu_initialize(CPU * const cpu); // initializes CPU registers and memory

void cpu_loadFontSet(CPU * const cpu); // loads font set into first 80 memory addresses

void cpu_zeroMemory(CPU * const cpu); // initializes all of memory to zero

void cpu_clearDisplay(CPU * const cpu); // clears (zeroes the display)

void cpu_run(CPU * const cpu);

void cpu_emulateCycle(CPU * const cpu); // perfoerms fetch, decode, execute, and timer update

void cpu_loadROM(CPU * const cpu, char const * const game_file); //loads the game into memory

void cpu_decodeAndExecute(CPU * const cpu); // handles decode and execute cycle by calling appropriate instructions.

void (*c8_instruction_table[16])(CPU * const cpu); // array of pointers to instruction functions

void c8_0xxx_instruction(CPU * const cpu); // handles instructions with high-order nibble 0

void c8_8xxx_instruction(CPU * const cpu); // handles instructions with high-order nibble 8

void c8_Exxx_instruction(CPU * const cpu); // handles instructions with high-order nibble E

void c8_Fxxx_instruction(CPU * const cpu); // handles instructions with high-order nibble F