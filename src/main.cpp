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

enum struct COLOR_SCHEMS : uint16_t
{
    DEFAULT = 1,
    SELECTED,

    TEXT_FILE,
    BINARY_FILE,
    DIRECTORY
};

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

    /* initialize the color pairs */
    init_pair((uint16_t) COLOR_SCHEMS::DEFAULT, COLOR_WHITE, COLOR_BLACK);
    init_pair((uint16_t) COLOR_SCHEMS::SELECTED, COLOR_GREEN, COLOR_BLACK);

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
        /* handle unexpected error */
        error.print();
        getch();
        std::exit(1);
    }
    /* end if - did error occure  */

    /* create all components */
    ui::component parent_tree([&content_parent_dir, &current_dir]() -> void {
        constexpr uint64_t origin_x = 0, origin_y = 0;

        /* move the cursor to the origin */
        move(origin_y, origin_x);

        /* change font color to default */
        attron(COLOR_PAIR(COLOR_SCHEMS::DEFAULT));

        /* for - iterate ocer content_parent_dir */
        for (std::vector<boost::filesystem::path>::iterator it = content_parent_dir.begin();
             it < content_parent_dir.end(); it++)
        {
            addnstr(boost::filesystem::relative(*it, current_dir.parent_path()).native().c_str(),
                    29);

            /* re-adjust the cursor */
            move(it - content_parent_dir.begin() + 1, 0);
        }
        /* end for - iterate ocer content_parent_dir */
    });
    ui::component current_tree([&content_current_dir, &selected_entry, &current_dir]() -> void {
        constexpr uint64_t origin_x = 30, origin_y = 0;

        /* move the cursor to the origin */
        move(origin_y, origin_x);

        /* for - iterate over content_current_dir */
        for (std::vector<boost::filesystem::path>::iterator it = content_current_dir.begin();
             it < content_current_dir.end(); it++)
        {
            /* if - current selected entry to be rendered */
            if (selected_entry == *it)
            {
                /* change font color to green */
                attron(COLOR_PAIR(COLOR_SCHEMS::SELECTED));
            }
            /* end if - current selected entry to be rendered */

            addnstr(boost::filesystem::relative(*it, current_dir).native().c_str(), 29);

            /* reset font color to default */
            attron(COLOR_PAIR(COLOR_SCHEMS::DEFAULT));

            /* re-adjust the cursor */
            move(it - content_current_dir.begin() + 1, 30);
        }
        /* end for - iterate over content_current_dir */
    });
    ui::component preview_tab(
        [&content_child_dir, &file_preview, &directory_selected, &selected_entry, &win]() -> void {
            constexpr uint64_t origin_x = 60, origin_y = 0;

            /* move the cursor to the origin */
            move(origin_y, origin_x);

            /* set the color mpde to default */
            attron(COLOR_PAIR(COLOR_SCHEMS::DEFAULT));

            /* if - is a directory currently selected */
            if (directory_selected)
            {
                /* for - iterate over every entry in the selected directory */
                for (std::vector<boost::filesystem::path>::iterator it = content_child_dir.begin();
                     it < content_child_dir.end(); it++)
                {
                    addnstr(boost::filesystem::relative(*it, selected_entry).native().c_str(), 29);

                    /* re-adjust the cursor */
                    move(it - content_child_dir.begin() + 1, origin_x);
                }
                /* end for - iterate over every entry in the selected directory */
            }
            else
            {
                uint64_t height = 0, width = 0, line_break_counts = 0, char_line_count = 0;
                /* get the terminal size */
                getmaxyx(win, height, width);

                /* for - iterate over every char in the preview */
                for (auto&& ch : file_preview)
                {
                    /* if - char is line break */
                    if (ch == '\n')
                    {
                        line_break_counts++;
                        char_line_count = 0;
                        continue;
                    }
                    /* end if - char is line break */

                    /* if - gonna overflow terminal */
                    if (char_line_count > width - origin_x - 1)
                    {
                        line_break_counts++;
                        char_line_count = 4;
                    }
                    /* end if - gonna overflow terminal */

                    /* re-adjust the cursor */
                    move(line_break_counts, 60 + char_line_count);
                    addch(ch);
                    char_line_count++;
                    if (line_break_counts > height - 1)
                    {
                        break;
                    }
                }
                /* end for - iterate over every char in the preview */
            }
            /* end if - is a directory currently selected */
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
