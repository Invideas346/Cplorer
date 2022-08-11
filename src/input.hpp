//
// Created by wolfgang on 07/08/2022.
//

#pragma once

#include <stdint.h>
#include "ascii_table.hpp"

typedef struct input {
    /* 4 * 64 = 256 (all ascii chars) */
    uint64_t keys[4];
} input;

input init_input();

void fetch_input(input* input);
uint8_t key_pressed_input(input* input, ascii_table character);