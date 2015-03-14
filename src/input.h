#pragma once

#include <stdint.h>
#include "CPU.h"

uint8_t wait_for_key(void);
void update_keys(CPU * const cpu);
void draw_to_screen(CPU * const cpu);