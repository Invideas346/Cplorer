//
// Created by wolfgang on 07/08/2022.
//

#include "input.h"

#if _WIN32
#include <ncurses/ncurses.h>
#elif unix
#include <curses.h>
#endif

static inline void clear_keys(input* input)
{
    for (uint32_t i = 0; i < 4; ++i)
        input->keys[i] = 0;
}

input init_input()
{
    input result;
    clear_keys(&result);
    return result;
}

void fetch_input(input* input)
{
    clear_keys(input);
    unsigned char first_char = getch();
    input->keys[first_char / 64] |= ((uint64_t) 1 << (first_char % 64));
}

uint8_t key_pressed_input(input* input, ascii_table character)
{
    return ((input->keys[character / 64] & ((uint64_t) 1 << (character % 64))) > 0);
}