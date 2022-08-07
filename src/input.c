//
// Created by wolfgang on 07/08/2022.
//

#include "input.h"

#if _WIN32

#include <stdlib.h>
#include <ncurses/ncurses.h>

static inline void clear_keys(input* input)
{
    for (uint32_t i = 0; i < 4; ++i)
        input->keys[i] = 0;
}

input init_input()
{
    input result;

    /* 4 * 64 = 256 (all ascii chars) */
    result.keys = (uint64_t*) malloc(sizeof(uint64_t) * 4);
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

#elif unix

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>

static inline void clear_keys(input* input)
{
    for (uint32_t i = 0; i < 4; ++i)
        input->keys[i] = 0;
}

input init_input()
{
    input result;

    /* 4 * 64 = 256 (all ascii chars) */
    result.keys = (uint64_t*) malloc(sizeof(uint64_t) * 4);
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

#endif