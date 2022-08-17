//
// Created by wolfgang on 07/08/2022.
//

/*<================= Include =================>*/
/*<--------------- Application --------------->*/
#include "main.hpp"
#include "ascii_table.hpp"
#include "input.hpp"
#include "fs_handler.hpp"
#include "cli.hpp"

/*<----------------- Library ----------------->*/
#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"

int main(int argc, char** argv)
{
    plog::init(plog::debug, "Log.txt");
    fs::error error;
    std::vector<boost::filesystem::path> content_current_dir, content_parent_dir, content_child_dir;
    boost::filesystem::path selected_entry, parent_dir;
    boost::filesystem::path current_dir;
    std::string file_preview;
    uint64_t selected_entry_index = 0;
    bool should_close = false;
    bool directory_selected = true;

    input input = init_input();

    /* init ncurses */
    WINDOW* win = initscr();

    /* set all ncurses functions to be non-blocking */
    nodelay(win, TRUE);

    curs_set(0);

    /* disable echoing input to stdout */
    noecho();

    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    /* get the content of the current and parent directory */
    current_dir = boost::filesystem::current_path();
    content_current_dir = fs::get_dir_content(current_dir, std::nullopt);
    content_parent_dir = fs::get_dir_content(current_dir.parent_path(), std::nullopt);
    selected_entry = boost::filesystem::path(content_current_dir[0]);
    content_child_dir = fs::get_dir_content(selected_entry, error);

    /* if - did error occure */
    if (error.ec == fs::error::INVALID_ARGUMENT)
    {
        directory_selected = false;
        file_preview = fs::get_file_content_n(selected_entry, 10, std::nullopt);
    }
    else if (error.ec != fs::error::NO_ERROR)
    {
        error.print();
        getch();
        std::exit(1);
    }
    /* end if - did error occure  */

    /* create all components */
    ui::component parent_tree([&content_parent_dir, &current_dir]() -> void {
        move(0, 0);
        for (std::vector<boost::filesystem::path>::iterator it = content_parent_dir.begin();
             it < content_parent_dir.end(); it++)
        {
            attron(COLOR_PAIR(1));
            addnstr(boost::filesystem::relative(*it, current_dir.parent_path()).native().c_str(),
                    29);
            move(it - content_parent_dir.begin() + 1, 0);
        }
    });
    ui::component current_tree([&content_current_dir, &selected_entry, &current_dir]() -> void {
        move(0, 30);
        for (std::vector<boost::filesystem::path>::iterator it = content_current_dir.begin();
             it < content_current_dir.end(); it++)
        {
            if (selected_entry == *it)
            {
                attron(COLOR_PAIR(2));
            }
            addnstr(boost::filesystem::relative(*it, current_dir).native().c_str(), 29);
            attron(COLOR_PAIR(1));
            move(it - content_current_dir.begin() + 1, 30);
        }
    });
    ui::component preview_tab(
        [&content_child_dir, &file_preview, &directory_selected, &selected_entry, &win]() -> void {
            move(0, 60);
            attron(COLOR_PAIR(1));
            if (directory_selected)
            {
                for (std::vector<boost::filesystem::path>::iterator it = content_child_dir.begin();
                     it < content_child_dir.end(); it++)
                {
                    attron(COLOR_PAIR(1));
                    addnstr(boost::filesystem::relative(*it, selected_entry).native().c_str(), 29);
                    move(it - content_child_dir.begin() + 1, 60);
                }
            }
            else
            {
                uint64_t height = 0, width = 0, line_break_counts = 0, char_line_count = 0;
                move(0, 60);
                getmaxyx(win, height, width);
                for (auto&& ch : file_preview)
                {
                    if (ch == '\n')
                    {
                        line_break_counts++;
                        char_line_count = 0;
                        continue;
                    }
                    if (char_line_count > width - 60 - 1)
                    {
                        line_break_counts++;
                        char_line_count = 4;
                    }
                    move(line_break_counts, 60 + char_line_count);
                    addch(ch);
                    char_line_count++;
                    if (line_break_counts > height - 1)
                    {
                        break;
                    }
                }
            }
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
        fetch_input(input);

        /* if - was enter pressed */
        if (key_pressed_input(input, CR) || key_pressed_input(input, L_LOWER) ||
            key_pressed_input(input, L_UPPER))
        {
            /* if - is the current selected entry a directory */
            if (directory_selected)
            {
                content_current_dir = content_child_dir;
                current_dir = boost::filesystem::path(selected_entry);
                content_parent_dir = fs::get_dir_content(current_dir.parent_path(), std::nullopt);
                selected_entry = content_current_dir[0];
                if (boost::filesystem::is_directory(selected_entry))
                {
                    content_child_dir = fs::get_dir_content(selected_entry, std::nullopt);
                }
                else
                {
                    file_preview = fs::get_file_content_n(selected_entry, 1000, std::nullopt);
                }
                selected_entry_index = 0;
            }
            /* end if - is the current selected entry a directory */
            clear();
        }
        /* end if - was enter pressed */

        /* if - was h pressed */
        if (key_pressed_input(input, H_LOWER) || key_pressed_input(input, H_UPPER))
        {
            /* if - has the current directory a parent directory */
            if (current_dir.has_parent_path())
            {
                /* get the content of the current and parent directory */
                content_child_dir = content_current_dir;
                current_dir = current_dir.parent_path();
                content_current_dir = fs::get_dir_content(current_dir, std::nullopt);
                content_parent_dir = fs::get_dir_content(current_dir.parent_path(), std::nullopt);
                selected_entry = content_current_dir[0];
                selected_entry_index = 0;
                clear();
            }
            /* end if - has the current directory a parent directory */
        }
        /* end if - was h pressed */

        /* if - was j pressed */
        if (key_pressed_input(input, J_LOWER) || key_pressed_input(input, J_UPPER))
        {
            /* if - stay inbounds of the content_current_dir vector */
            if (selected_entry_index < content_current_dir.size() - 1)
            {
                selected_entry = content_current_dir[selected_entry_index + 1];
                selected_entry_index++;
                directory_selected = boost::filesystem::is_directory(selected_entry) ? true : false;
                if (directory_selected)
                {
                    content_child_dir = fs::get_dir_content(selected_entry, std::nullopt);
                }
                else
                {
                    file_preview = fs::get_file_content_n(selected_entry, 1000, std::nullopt);
                }
                clear();
            }
            /* end if - stay inbounds of the content_current_dir vector */
        }
        /* end if - was j pressed */

        /* end if - was k pressed */
        if (key_pressed_input(input, K_LOWER) || key_pressed_input(input, K_UPPER))
        {
            /* if - stay inbounds of the content_current_dir vector */
            if (selected_entry_index > 0)
            {
                selected_entry = content_current_dir[selected_entry_index - 1];
                selected_entry_index--;
                directory_selected = boost::filesystem::is_directory(selected_entry) ? true : false;
                if (directory_selected)
                {
                    content_child_dir = fs::get_dir_content(selected_entry, std::nullopt);
                }
                else
                {
                    file_preview = fs::get_file_content_n(selected_entry, 1000, std::nullopt);
                }
                clear();
            }
            /* end if - stay inbounds of the content_current_dir vector */
        }
        /* end if - was k pressed */

        /* if - was ESC pressed */
        if (key_pressed_input(input, ESC))
        {
            should_close = true;
        }
        /* end if - was ESC pressed */

        /* render the ui components */
        ui_tree.render();
    }
    /* end while - application loop */

    /* de-init ncurses */
    endwin();
    return 0;
}
