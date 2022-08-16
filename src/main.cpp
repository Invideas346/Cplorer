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

/* TODO: Change all std::string paths to boost::filesystem::path's */
int main(int argc, char** argv)
{
    plog::init(plog::debug, "Log.txt");
    fs::error error;
    std::vector<std::string> content_current_dir, content_parent_dir, content_child_dir;
    std::string m_file_preview;
    boost::filesystem::path selected_entry, parent_dir;
    boost::filesystem::path current_dir;
    uint64_t selected_entry_index = 0;
    bool should_close = false;
    bool directory_selected = true;

    input input = init_input();

    /* init ncurses */
    WINDOW* win = initscr();

    /* set all ncurses functions to be non-blocking */
    nodelay(win, TRUE);

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

    /* convert the absolut path to an relative path */
    for (auto&& entry : content_parent_dir)
    {
        if (entry.find_last_of('/') != std::string::npos)
        {
            entry = entry.substr(entry.find_last_of('/'), entry.size() - entry.find_last_of('/'));
        }
    }
    for (auto&& entry : content_current_dir)
    {
        if (entry.find_last_of('/') != std::string::npos)
        {
            entry = entry.substr(entry.find_last_of('/'), entry.size() - entry.find_last_of('/'));
        }
    }

    /* if - did error occure */
    if (error.ec == fs::error::INVALID_ARGUMENT)
    {
        directory_selected = false;
        m_file_preview = fs::get_file_content_n(selected_entry, 10, std::nullopt);
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
        move(0, 0);
        for (std::vector<std::string>::iterator it = content_parent_dir.begin();
             it < content_parent_dir.end(); it++)
        {
            attron(COLOR_PAIR(1));
            addnstr((*it).c_str(), 29);
            move(it - content_parent_dir.begin() + 1, 0);
        }
    });
    ui::component current_tree([&content_current_dir, &selected_entry]() -> void {
        move(0, 30);
        for (std::vector<std::string>::iterator it = content_current_dir.begin();
             it < content_current_dir.end(); it++)
        {
            if (selected_entry == *it)
            {
                attron(COLOR_PAIR(2));
            }
            addnstr((*it).c_str(), 29);
            attron(COLOR_PAIR(1));
            move(it - content_current_dir.begin() + 1, 30);
        }
    });
    ui::component preview_tab([&content_child_dir, &m_file_preview, &directory_selected]() -> void {
        move(0, 60);
        attron(COLOR_PAIR(1));
        addnstr(m_file_preview.c_str(), 29);
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
            if (directory_selected)
            {
                current_dir = boost::filesystem::path(selected_entry);
                content_current_dir = fs::get_dir_content(current_dir, std::nullopt);
                content_parent_dir = fs::get_dir_content(current_dir.parent_path(), std::nullopt);

                /* convert the absolut path to an relative path */
                for (auto&& entry : content_parent_dir)
                {
                    if (entry.find_last_of('/') != std::string::npos)
                    {
                        entry = entry.substr(entry.find_last_of('/'),
                                             entry.size() - entry.find_last_of('/'));
                    }
                }
                for (auto&& entry : content_current_dir)
                {
                    if (entry.find_last_of('/') != std::string::npos)
                    {
                        entry = entry.substr(entry.find_last_of('/'),
                                             entry.size() - entry.find_last_of('/'));
                    }
                }
                selected_entry = content_current_dir[0];
                content_child_dir = fs::get_dir_content(selected_entry, error);
            }
            clear();
        }
        /* end if - was enter pressed */

        /* if - was h pressed */
        if (key_pressed_input(input, H_LOWER) || key_pressed_input(input, H_UPPER))
        {
            /* get the content of the current and parent directory */
            current_dir = current_dir.parent_path();
            content_current_dir = fs::get_dir_content(current_dir, std::nullopt);
            content_parent_dir = fs::get_dir_content(current_dir.parent_path(), std::nullopt);

            /* convert the absolut path to an relative path */
            for (auto&& entry : content_parent_dir)
            {
                if (entry.find_last_of('/') != std::string::npos)
                {
                    entry = entry.substr(entry.find_last_of('/'),
                                         entry.size() - entry.find_last_of('/'));
                }
            }
            for (auto&& entry : content_current_dir)
            {
                if (entry.find_last_of('/') != std::string::npos)
                {
                    entry = entry.substr(entry.find_last_of('/'),
                                         entry.size() - entry.find_last_of('/'));
                }
            }
            selected_entry = content_current_dir[0];
            content_child_dir = fs::get_dir_content(selected_entry, error);
            clear();
        }
        /* end if - was h pressed */

        /* if - was j pressed */
        if (key_pressed_input(input, J_LOWER) || key_pressed_input(input, J_UPPER))
        {
            if (selected_entry_index < content_current_dir.size() - 1)
            {
                selected_entry = content_current_dir[selected_entry_index + 1];
                selected_entry_index++;
                directory_selected = boost::filesystem::is_directory(selected_entry) ? true : false;
            }
            clear();
        }
        /* end if - was j pressed */

        /* end if - was k pressed */
        if (key_pressed_input(input, K_LOWER) || key_pressed_input(input, K_UPPER))
        {
            if (selected_entry_index > 0)
            {
                selected_entry = content_current_dir[selected_entry_index - 1];
                selected_entry_index--;
                directory_selected = boost::filesystem::is_directory(selected_entry) ? true : false;
            }
            clear();
        }
        /* end if - was k pressed */

        /* if - was l pressed */
        if (key_pressed_input(input, L_LOWER) || key_pressed_input(input, L_UPPER))
        {
            should_close = true;
        }
        /* end if - was l pressed */

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
