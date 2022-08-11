//
// Created by wolfgang on 07/08/2022.
//

#include "main.hpp"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <boost/filesystem.hpp>

#if WIN32
#include <ncurses/ncurses.h>
#elif unix
#include <curses.h>
#endif

#include "ascii_table.hpp"
#include "input.hpp"
#include "fs_handler.hpp"
#include "cli.hpp"

int main(int argc, char** argv)
{
    fs::ERROR_CODE ec;
    std::vector<std::string> content_current_dir, content_parent_dir, content_child_dir;
    std::string m_file_preview;
    boost::filesystem::directory_entry selected_entry;
    uint8_t should_close = false;

    ui::component component;
    component.attach_render_routine(
        []() -> void {

        });

    input input = init_input();

    /* init ncurses */
    initscr();

    /* disable echoing input to stdout */
    noecho();

    /* get the content of the current and parent directory */
    content_current_dir = fs::get_dir_content(boost::filesystem::current_path());
    content_parent_dir = fs::get_dir_content(boost::filesystem::current_path().parent_path());
    content_child_dir = fs::get_dir_content(selected_entry, &ec);
    if (ec == fs::ERROR_CODE::INVALID_ARGUMENT)
    {
        m_file_preview = fs::get_file_content(selected_entry);
    }

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
