//
// Created by wolfgang on 07/08/2022.
//

/*<===================== Includes =====================>*/
/*<---------------- System-Includes ------------------->*/
/*<-------------- Application-Includes ---------------->*/
#include "main.hpp"
#include "ascii_table.hpp"
#include "input.hpp"
#include "fs_handler.hpp"
#include "cli.hpp"

/*<----------------- Library-Includes ----------------->*/
#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

enum struct COLOR_SCHEME : uint16_t
{
    TEXT_FILE = 1,
    BINARY_FILE,
    DIRECTORY,
    PNG_FILE,

    TEXT_FILE_SELECTED,
    BINARY_FILE_SELECTED,
    DIRECTORY_SELECTED,
    PNG_FILE_SELECTED,

    DEFAULT_TEXT
};

static inline COLOR_SCHEME determin_COLOR_SCHEME(boost::filesystem::path path,
                                                 std::optional<bool> is_selected)
{
    if (!is_selected.value())
    {
        if (boost::filesystem::is_regular_file(path))
        {
            return COLOR_SCHEME::TEXT_FILE;
        }
        else if (boost::filesystem::is_directory(path))
        {
            return COLOR_SCHEME::DIRECTORY;
        }
        else if (boost::filesystem::extension(path) == "png" ||
                 boost::filesystem::extension(path) == "PNG")
        {
            return COLOR_SCHEME::PNG_FILE;
        }
        else
        {
            return COLOR_SCHEME::BINARY_FILE;
        }
    }
    else
    {
        if (boost::filesystem::is_regular_file(path))
        {
            return COLOR_SCHEME::TEXT_FILE_SELECTED;
        }
        else if (boost::filesystem::is_directory(path))
        {
            return COLOR_SCHEME::DIRECTORY_SELECTED;
        }
        else if (boost::filesystem::extension(path) == "png" ||
                 boost::filesystem::extension(path) == "PNG")
        {
            return COLOR_SCHEME::PNG_FILE_SELECTED;
        }
        else
        {
            return COLOR_SCHEME::BINARY_FILE_SELECTED;
        }
    }
}

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

    /* disable the cursor */
    curs_set(0);

    /* disable echoing input to stdout */
    noecho();

    /* initialize the color pairs */
    start_color();
    init_pair((uint16_t) COLOR_SCHEME::TEXT_FILE, COLOR_WHITE, COLOR_BLACK);
    init_pair((uint16_t) COLOR_SCHEME::BINARY_FILE, COLOR_GREEN, COLOR_BLACK);
    init_pair((uint16_t) COLOR_SCHEME::DIRECTORY, COLOR_BLUE, COLOR_BLACK);
    init_pair((uint16_t) COLOR_SCHEME::PNG_FILE, COLOR_YELLOW, COLOR_BLACK);
    init_pair((uint16_t) COLOR_SCHEME::TEXT_FILE_SELECTED, COLOR_BLACK, COLOR_WHITE);
    init_pair((uint16_t) COLOR_SCHEME::BINARY_FILE_SELECTED, COLOR_BLACK, COLOR_GREEN);
    init_pair((uint16_t) COLOR_SCHEME::DIRECTORY_SELECTED, COLOR_BLACK, COLOR_BLUE);
    init_pair((uint16_t) COLOR_SCHEME::PNG_FILE_SELECTED, COLOR_BLACK, COLOR_YELLOW);
    init_pair((uint16_t) COLOR_SCHEME::DEFAULT_TEXT, COLOR_WHITE, COLOR_BLACK);

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
    ui::component parent_tree(
        [&content_parent_dir, &current_dir, &win]() -> void
        {
            uint32_t width = 0, height = 0;

            /* get window heigth and width */
            getmaxyx(win, height, width);

            /* if - no parent - abort */
            if (!current_dir.has_parent_path())
            {
                return;
            }
            /* end if - no parent - abort */

            constexpr uint64_t origin_x = 0, origin_y = 0;

            /* move the cursor to the origin */
            move(origin_y, origin_x);

            /* if - parent on top of parent tree */
            if (current_dir.parent_path() != content_parent_dir[0])
            {
                /* place parent path on top of parent tree */
                auto iter =
                    std::find_if(content_parent_dir.begin(), content_parent_dir.end(),
                                 [&current_dir](const boost::filesystem::path& entry) -> bool
                                 { return entry == current_dir; });
                std::swap(content_parent_dir[0], *iter);
            }
            /* end if - parent on top of parent tree */

            uint32_t line_count = 0;

            /* for - iterate over content_parent_dir */
            for (std::vector<boost::filesystem::path>::iterator it = content_parent_dir.begin();
                 it < content_parent_dir.end(); it++)
            {
                /* if - line limit reached */
                if (line_count > height - 2)
                {
                    /* break in order to not overwrite the bottom bar */
                    break;
                }
                /* end if - line limit reached */

                COLOR_SCHEME scheme = determin_COLOR_SCHEME(*it, *it == current_dir);
                attron(COLOR_PAIR(scheme));
                addnstr(
                    boost::filesystem::relative(*it, current_dir.parent_path()).native().c_str(),
                    29);
                uint32_t path_length =
                    boost::filesystem::relative(*it, current_dir.parent_path()).native().size();

                /* if - does the row have to filled */
                if (path_length < 28)
                {
                    /* for - file row */
                    for (size_t i = path_length; i < 28; i++)
                    {
                        addch(' ');
                    }
                    /* end for - file row */
                }
                /* end if - does the row have to filled */

                line_count++;

                /* re-adjust the cursor */
                move(it - content_parent_dir.begin() + 1, 0);
            }
            /* end for - iterate over content_parent_dir */
        });
    ui::component current_tree(
        [&content_current_dir, &selected_entry, &current_dir, &win]() -> void
        {
            uint32_t width = 0, height = 0;

            /* get window heigth and width */
            getmaxyx(win, height, width);

            constexpr uint64_t origin_x = 30, origin_y = 0;

            /* move the cursor to the origin */
            move(origin_y, origin_x);

            uint32_t line_count = 0;

            /* for - iterate over content_current_dir */
            for (std::vector<boost::filesystem::path>::iterator it = content_current_dir.begin();
                 it < content_current_dir.end(); it++)
            {
                /* if - line limit reached */
                if (line_count > height - 2)
                {
                    /* break in order to not overwrite the bottom bar */
                    break;
                }
                /* end if - line limit reached */

                COLOR_SCHEME scheme = determin_COLOR_SCHEME(*it, *it == selected_entry);
                attron(COLOR_PAIR(scheme));
                addnstr(boost::filesystem::relative(*it, current_dir).native().c_str(), 29);
                uint32_t path_length =
                    boost::filesystem::relative(*it, current_dir).native().size();

                /* if - is entry directroy */
                if (boost::filesystem::is_directory(*it))
                {
                    uint32_t digit_count = 0;
                    uint32_t children_count = fs::get_children_count(*it, std::nullopt);

                    /* for - count digets to be printed */
                    for (uint32_t i = 1;; i *= 10)
                    {
                        /* if - still bigger */
                        if (children_count / i >= 1)
                        {
                            digit_count++;
                            continue;
                        }
                        /* end if - still bigger */

                        break;
                    }
                    /* end for - count digets to be printed */

                    /* if - does the row have to filled */
                    if (path_length < 28)
                    {
                        /* for - print whitespaces */
                        for (size_t i = path_length; i < 28; i++)
                        {
                            /* if - print whitespace or child count */
                            if (28 - i > digit_count)
                            {
                                addch(' ');
                            }
                            else
                            {
                                printw("%d", children_count);
                                break;
                            }
                            /* end if - print whitespace or child count */
                        }
                        /* end for - print whitespaces */
                    }
                    /* end if - does the row have to filled */
                }
                else
                {
                    /* if - does the row have to filled */
                    if (path_length < 28)
                    {
                        /* for - print whitespaces */
                        for (size_t i = path_length; i < 28; i++)
                        {
                            addch(' ');
                        }
                        /* end for - print whitespaces */
                    }
                    /* end if - does the row have to filled */
                }
                /* end if - is entry directroy */

                line_count++;

                /* re-adjust the cursor */
                move(it - content_current_dir.begin() + 1, 30);
            }
            /* end for - iterate over content_current_dir */
        });
    ui::component preview_tab(
        [&content_child_dir, &file_preview, &directory_selected, &selected_entry, &win]() -> void
        {
            uint32_t width = 0, height = 0;
            uint32_t line_count = 0;

            /* get window heigth and width */
            getmaxyx(win, height, width);

            constexpr uint64_t origin_x = 60, origin_y = 0;

            /* move the cursor to the origin */
            move(origin_y, origin_x);

            /* if - is a directory currently selected */
            if (directory_selected)
            {
                /* for - iterate over every entry in the selected directory */
                for (std::vector<boost::filesystem::path>::iterator it = content_child_dir.begin();
                     it < content_child_dir.end(); it++)
                {
                    /* if - line limit reached */
                    if (line_count > height - 2)
                    {
                        /* break in order to not overwrite the bottom bar */
                        break;
                    }
                    /* end if - line limit reached */

                    COLOR_SCHEME scheme = determin_COLOR_SCHEME(*it, *it == content_child_dir[0]);
                    attron(COLOR_PAIR(scheme));

                    addnstr(boost::filesystem::relative(*it, selected_entry).native().c_str(), 29);
                    uint32_t path_length =
                        boost::filesystem::relative(*it, selected_entry.parent_path())
                            .native()
                            .size();

                    /* if - does the row have to filled */
                    if (path_length < 28)
                    {
                        /* for - print whitespaces */
                        for (size_t i = path_length; i < 28; i++)
                        {
                            addch(' ');
                        }
                        /* end for - print whitespaces */
                    }
                    /* end if - does the row have to filled */

                    line_count++;

                    /* re-adjust the cursor */
                    move(it - content_child_dir.begin() + 1, origin_x);
                }
                /* end for - iterate over every entry in the selected directory */
            }
            else
            {
                uint64_t height = 0, width = 0, line_count = 0, char_line_count = 0;
                /* get the terminal size */
                getmaxyx(win, height, width);

                /* set the color mpde to default */
                attron(COLOR_PAIR(COLOR_SCHEME::DEFAULT_TEXT));

                /* for - iterate over every char in the preview */
                for (auto&& ch : file_preview)
                {
                    /* if - screen already full */
                    if (line_count > height - 2)
                    {
                        break;
                    }
                    /* end if - screen already full */

                    /* if - char is line break */
                    if (ch == '\n')
                    {
                        line_count++;
                        char_line_count = 0;
                        continue;
                    }
                    /* end if - char is line break */

                    /* if - gonna overflow terminal */
                    if (char_line_count > width - origin_x - 1)
                    {
                        line_count++;
                        char_line_count = 4;
                    }
                    /* end if - gonna overflow terminal */

                    /* re-adjust the cursor */
                    move(line_count, 60 + char_line_count);
                    addch(ch);
                    char_line_count++;
                }
                /* end for - iterate over every char in the preview */
            }
            /* end if - is a directory currently selected */
        });
    ui::component bottom_bar(
        [&win, &selected_entry, &directory_selected]()
        {
            std::string descriptor = "";
            uint32_t width = 0, height = 0;

            /* get the terminal size */
            getmaxyx(win, height, width);

            const uint64_t origin_x = 0, origin_y = height - 1;

            /* move the cursor to the origin */
            move(origin_y, origin_x);

            /* if - directory selected */
            if (directory_selected)
            {
                descriptor.append("d");
            }
            /* end if - directory selected */

            boost::filesystem::perms perms =
                boost::filesystem::status(selected_entry).permissions();

            /* for - iterate over owner, group and other group */
            for (int32_t i = 100; i >= 1; i /= 10)
            {
                /*
                    0x4 - Read
                    0x2 - Write
                    0x1 - Execute
                 */
                /* extract digit */
                uint32_t digit = (perms / i) % 10;

                /* test for permissions */
                (digit & 0x4) ? descriptor.append("r") : descriptor.append("-");
                (digit & 0x2) ? descriptor.append("w") : descriptor.append("-");
                (digit & 0x1) ? descriptor.append("x") : descriptor.append("-");
            }
            /* end for - iterate over owner, group and other group */

            /* print result */
            attron(COLOR_PAIR(COLOR_SCHEME::DEFAULT_TEXT));
            printw("%s", descriptor.c_str());
        });

    /* create the component tree and add all components */
    ui::component_tree ui_tree;
    ui_tree.add_comp(parent_tree);
    ui_tree.add_comp(current_tree);
    ui_tree.add_comp(preview_tab);
    ui_tree.add_comp(bottom_bar);

    /* while - application loop */
    while (!should_close)
    {
        /* get console input */
        fetch_input(input);

        /* if - was enter or l pressed */
        if (key_pressed_input(input, CR) || key_pressed_input(input, L_LOWER) ||
            key_pressed_input(input, L_UPPER))
        {
            /* if - is the current selected entry a directory */
            if (directory_selected)

            {
                /* current content => parent content */
                content_parent_dir = content_current_dir;

                /* child content => current content */
                content_current_dir = content_child_dir;

                /* prev selected entry to current dir */
                current_dir = boost::filesystem::path(selected_entry);

                /* always select the first entry */
                selected_entry = content_current_dir[0];
                selected_entry_index = 0;

                /* if - entry directory */
                if (boost::filesystem::is_directory(selected_entry))
                {
                    content_child_dir = fs::get_dir_content(selected_entry, std::nullopt);
                    directory_selected = true;
                }
                else
                {
                    file_preview.clear();
                    file_preview = fs::get_file_content_n(selected_entry, 5000, std::nullopt);
                    directory_selected = false;
                }
                /* end if - entry directory */
            }
            /* end if - is the current selected entry a directory */

            /* clear screen to re-render */
            clear();
        }
        /* end if - was enter or l pressed */

        /* if - was h pressed */
        if (key_pressed_input(input, H_LOWER) || key_pressed_input(input, H_UPPER))
        {
            /* if - has the current directory a parent directory */
            if (current_dir.has_parent_path())
            {
                /* current content  => child content */
                content_child_dir = content_current_dir;
                /* parent content  => current content */
                content_current_dir = content_parent_dir;

                /* can only be parented by directory */
                directory_selected = true;

                /* prev. parent must be selected */
                selected_entry = selected_entry.parent_path();
                current_dir = current_dir.parent_path();

                /* get parent content*/
                content_parent_dir = fs::get_dir_content(current_dir.parent_path(), std::nullopt);

                /* for - iterate over new current content */
                for (std::vector<boost::filesystem::path>::iterator it =
                         content_current_dir.begin();
                     it < content_current_dir.end(); it++)
                {
                    /* find new selected entry and get index */
                    auto found_iter = std::find(content_current_dir.begin(),
                                                content_current_dir.end(), selected_entry);
                    selected_entry_index = found_iter - content_current_dir.begin();
                }
                /* end for - iterate over new current content */

                /* clear screen to re-render */
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
                /* move down in tree */
                selected_entry = content_current_dir[selected_entry_index + 1];
                selected_entry_index++;

                /* determine if directoryy selected */
                directory_selected = boost::filesystem::is_directory(selected_entry) ? true : false;

                /* if - directory selected */
                if (directory_selected)
                {
                    content_child_dir = fs::get_dir_content(selected_entry, std::nullopt);
                }
                else
                {
                    /* try to get preview of text-file */
                    file_preview.clear();
                    file_preview = fs::get_file_content_n(selected_entry, 5000, std::nullopt);
                }
                /* end if - directory selected */

                /* clear screen to re-render */
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
                /* move up in tree */
                selected_entry = content_current_dir[selected_entry_index - 1];
                selected_entry_index--;

                /* determine if directoryy selected */
                directory_selected = boost::filesystem::is_directory(selected_entry) ? true : false;

                /* if - directory selected */
                if (directory_selected)
                {
                    content_child_dir = fs::get_dir_content(selected_entry, std::nullopt);
                }
                else
                {
                    /* try to get preview of text-file */
                    file_preview.clear();
                    file_preview = fs::get_file_content_n(selected_entry, 5000, std::nullopt);
                }
                /* end if - directory selected */

                /* clear screen to re-render */
                clear();
            }
            /* end if - stay inbounds of the content_current_dir vector */
        }
        /* end if - was k pressed */

        /* if - was ESC pressed */
        if (key_pressed_input(input, ESC) || key_pressed_input(input, Q_LOWER) ||
            key_pressed_input(input, Q_UPPER))
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
    return EXIT_SUCCESS;
}