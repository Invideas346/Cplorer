//
// Created by wolfgang on 07/08/2022.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "ascii_table.h"
#include "dir_handler.h"
#include "file_handler.h"
#include "typedef.h"

int main()
{
    uint8_t should_close = false;

    /* while - application loop */
    while (!should_close)
    {
        /* get console input */
        uint8_t input = fgetc(stdin);

        /* switch - determine the input and act on it */
        switch (input)
        {
        case ESC:
            should_close = true;
            break;

        case H_UPPER:
        case H_LOWER:
            break;

        case J_UPPER:
        case J_LOWER:
            break;

        case K_UPPER:
        case K_LOWER:
            break;

        case L_UPPER:
        case L_LOWER:
            break;

        default:
            break;
        }
        /* end switch - determine the input and act on it */
    }
    /* end while - application loop */
    return 0;
}
