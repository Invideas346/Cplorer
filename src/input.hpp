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

typedef struct Input
{
    void init();
    void fetch();
    bool key_pressed_input(ascii_table character);

    /* 4 * 64 = 256 (all ascii chars) */
    uint64_t keys[4];

  private:
    void clear();
} Input;