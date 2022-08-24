#pragma once

/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <optional>

/*<-------------- Application-Includes ---------------->*/
/*<----------------- Library-Includes ----------------->*/
#include <boost/filesystem.hpp>
#if WIN32
#include <ncurses/ncurses.h>
#elif unix
#include <curses.h>
#endif