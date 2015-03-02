#pragma once

#include "chip8.h"

void c8_initialize(Chip8 * const my_chip8); // initializes chip8 registers and memory

void c8_loadFontSet(Chip8 * const my_chip8); // loads font set into first 80 memory addresses

void c8_zeroMemory(Chip8 * const my_chip8); // initializes all of memory to zero

void c8_clearDisplay(Chip8 * const my_chip8); // clears (zeroes the display)

void c8_emulateCycle(Chip8 * const my_chip8); // perfoerms fetch, decode, execute, and timer update

void c8_loadGame(Chip8 * const my_chip8, char const * const game_file); //loads the game into memory

void c8_decodeAndExecute(Chip8 * const my_chip8); // handles decode and execute cycle by calling appropriate instructions.