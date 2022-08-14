//
// Created by wolfgang on 07/08/2022.
//

#include "main.hpp"

#include "ascii_table.hpp"
#include "input.hpp"
#include "fs_handler.hpp"
#include "cli.hpp"

#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"

int main(int argc, char** argv)
{
    plog::init(plog::debug, "Log.txt");
    fs::error error;
    std::vector<std::string> content_current_dir, content_parent_dir, content_child_dir;
    std::string m_file_preview;
    boost::filesystem::directory_entry selected_entry;
    uint64_t selected_entry_index = 0;
    bool should_close = false;
    bool directory_selected = true;

    input input = init_input();

    /* init ncurses */
    initscr();

    /* disable echoing input to stdout */
    noecho();

    /* get the content of the current and parent directory */
    content_current_dir = fs::get_dir_content(boost::filesystem::current_path());
    content_parent_dir = fs::get_dir_content(boost::filesystem::current_path().parent_path());
    selected_entry = boost::filesystem::directory_entry(content_current_dir[0]);
    content_child_dir = fs::get_dir_content(selected_entry, &error);

    /* if - did error occure */
    if (error.ec == fs::error::INVALID_ARGUMENT)
    {
        directory_selected = false;
        m_file_preview =
            fs::get_file_content_n("/home/wolfgang/Documents/Cplorer/.clang-format", 10);
    }
    else if (error.ec != fs::error::NO_ERROR)
    {
        error.print();
        getch();
        std::exit(1);
    }
    /* end if - did error occure  */

    /* create all components */
    ui::component parent_tree([&content_parent_dir]() -> void {

    });
    ui::component current_tree([&content_current_dir]() -> void {

    });
    ui::component preview_tab([&content_child_dir, &m_file_preview, &directory_selected]() -> void {

    });

    /* create the component tree and add all components */
    ui::component_tree ui_tree;
    ui_tree.add_comp(parent_tree);
    ui_tree.add_comp(current_tree);
    ui_tree.add_comp(preview_tab);

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
            if (selected_entry_index < content_current_dir.size() - 2)
            {
                selected_entry = boost::filesystem::directory_entry(
                    content_current_dir[selected_entry_index + 1]);
                selected_entry_index++;
            }
        }
        /* end if - was j pressed */

        /* end if - was k pressed */
        if (key_pressed_input(&input, K_LOWER) || key_pressed_input(&input, K_UPPER))
        {
            if (selected_entry_index > 0)
            {
                selected_entry = boost::filesystem::directory_entry(
                    content_current_dir[selected_entry_index - 1]);
                selected_entry_index--;
            }
        }
        /* end if - was k pressed */

        /* if - was l pressed */
        if (key_pressed_input(&input, L_LOWER) || key_pressed_input(&input, L_UPPER))
        {
            should_close = true;
        }
        /* end if - was l pressed */

        ui_tree.render();
    }
    /* end while - application loop */

    /* de-init ncurses */
    endwin();
    return 0;
}
