//
// Created by wolfgang on 07/08/2022.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#if WIN32
#include <ncurses/ncurses.h>
#elif unix
#include <curses.h>
#endif


#include "ascii_table.h"
#include "dir_handler.h"
#include "file_handler.h"
#include "input.h"

int main()
{
    input input = init_input();
    uint8_t should_close = false;

    /* init ncurses */
    initscr();

    /* while - application loop */
    while (!should_close)
    {
        /* get console input */
        fetch_input(&input);

        /* if - was h pressed */
        if (key_pressed_input(&input, H_LOWER) || key_pressed_input(&input, H_UPPER))
        {
            should_close = true;
        }
        /* end if - was h pressed */

        /* if - was j pressed */
        if (key_pressed_input(&input, J_LOWER) || key_pressed_input(&input, J_UPPER))
        {
            should_close = true;
        }
        /* end if - was j pressed */

        /* end if - was k pressed */
        if (key_pressed_input(&input, K_LOWER) || key_pressed_input(&input, K_UPPER))
        {
            should_close = true;
        }
        /* end if - was k pressed */

        /* if - was l pressed */
        if (key_pressed_input(&input, L_LOWER) || key_pressed_input(&input, L_UPPER))
        {
            should_close = true;
        }
        /* end if - was l pressed */

        // render_parent_tree();
        // render_current_tree();
        // render_preview();
    }
    /* end while - application loop */

    /* de-init ncurses */
    endwin();
    return 0;
}
