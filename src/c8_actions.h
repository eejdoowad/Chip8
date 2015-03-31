#pragma once

#include "CPU.h"
#include "instructions.h"

// initializes CPU registers and memory
void cpu_initialize(CPU * const cpu);

// perfoerms fetch, decode, execute, and timer update
void cpu_emulateCycle(CPU * const cpu);

//loads the game into memory
int cpu_loadROM(CPU * const cpu, char const * const game_file);

// handles decode and execute cycle by calling appropriate instructions.
void cpu_decodeAndExecute(CPU * const cpu); 