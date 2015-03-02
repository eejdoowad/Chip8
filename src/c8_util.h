#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"

void c8_print_mem(Chip8 const * const c8, uint16_t first, uint16_t last);