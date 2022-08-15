#pragma once

/*<================= Include =================>*/
/*<----------------- System ------------------>*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/*<----------------- Library ----------------->*/
#include <boost/filesystem.hpp>
#if WIN32
#include <ncurses/ncurses.h>
#elif unix
#include <curses.h>
#endif