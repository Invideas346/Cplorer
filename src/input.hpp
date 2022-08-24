//
// Created by wolfgang on 07/08/2022.
//

#pragma once

/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
#include <cstdint>

/*<--------------- Application-Includes --------------->*/
#include "ascii_table.hpp"

/*<----------------- Library-Includes ----------------->*/

typedef struct input
{
    /* 4 * 64 = 256 (all ascii chars) */
    uint64_t keys[4];
} input;

input init_input();

void fetch_input(input& input);
uint8_t key_pressed_input(input& input, ascii_table character);