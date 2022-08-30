//
// Created by wolfgang on 07/08/2022.
//

/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
/*<--------------- Application-Includes --------------->*/
#include "input.hpp"

/*<----------------- Library-Includes ----------------->*/
#if _WIN32
#include <ncurses/ncurses.h>
#elif unix
#include <curses.h>
#endif

void input::clear()
{
    for (uint32_t i = 0; i < 4; ++i)
        this->keys[i] = 0;
}

void input::init() { this->clear(); }

void input::fetch()
{
    this->clear();
    unsigned char first_char = getch();
    this->keys[first_char / 64] |= ((uint64_t) 1 << (first_char % 64));
}

bool input::key_pressed_input(ascii_table character)
{
    return ((this->keys[character / 64] & ((uint64_t) 1 << (character % 64))) > 0);
}