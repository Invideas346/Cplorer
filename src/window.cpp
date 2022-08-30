/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
/*<-------------- Application-Includes ---------------->*/
#include "window.hpp"

/*<----------------- Library-Includes ----------------->*/

namespace UI
{
    void Window::init()
    {
        m_win = initscr();

        /* enable no-blocking getch */
        nodelay(m_win, TRUE);

        /* disable the cursor */
        curs_set(0);

        /* disable echoing input to stdout */
        noecho();
    }
    void Window::deinit() {}
    void Window::clear_scr() { clear(); }
    bool Window::was_resized()
    {
        uint32_t current_width = 0, current_height = 0;
        getmaxyx(m_win, current_height, current_width);
        if (m_height != current_height || m_width != current_width)
        {
            m_width = current_width;
            m_height = current_height;
            return true;
        }
        return false;
    }
    WindowSize Window::get_window_size() const { return {m_width, m_height}; }
} // namespace UI