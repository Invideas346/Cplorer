#pragma once

/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
#include <functional>

/*<-------------- Application-Includes ---------------->*/
/*<----------------- Library-Includes ----------------->*/
#if WIN32
#include <ncurses/ncurses.h>
#elif unix
#include <curses.h>
#endif

namespace ui
{

    struct window_size
    {
        uint32_t width, height;
    };

    struct window
    {
      public:
        window() = default;

        void init();
        void deinit();
        bool was_resized();

        window_size get_window_size() const;

      private:
        WINDOW* m_win = nullptr;
        uint32_t m_height, m_width;
    };
} // namespace ui