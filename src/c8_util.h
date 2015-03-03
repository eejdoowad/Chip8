#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "CPU.h"

void cpu_print_regs(CPU const * const cpu);
void cpu_print_screen(CPU const * const cpu);
void cpu_print_mem(CPU const * const cpu, uint16_t first, uint16_t last);
void cpu_print_everything(CPU const * const cpu, uint16_t first, uint16_t last);