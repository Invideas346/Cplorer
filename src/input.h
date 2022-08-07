//
// Created by wolfgang on 07/08/2022.
//

#ifndef CPLORER_INPUT_H
#define CPLORER_INPUT_H

#include <stdint.h>
#include "ascii_table.h"

typedef struct input {
    uint64_t* keys;
} input;

input init_input();

void fetch_input(input* input);
uint8_t key_pressed_input(input* input, ascii_table character);

#endif // CPLORER_INPUT_H
