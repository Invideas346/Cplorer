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

namespace UI
{

    struct WindowSize
    {
        uint32_t width, height;
    };

    struct Window
    {
      public:
        Window() = default;

        void init();
        void deinit();
        bool was_resized();

        void clear_scr();

        WindowSize get_window_size() const;

      private:
        WINDOW* m_win = nullptr;
        uint32_t m_height, m_width;
    };
} // namespace UI