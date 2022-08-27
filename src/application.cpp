
/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <optional>

/*<-------------- Application-Includes ---------------->*/
#include "application.hpp"
#include "config.hpp"

/*<----------------- Library-Includes ----------------->*/
#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <nlohmann/json.hpp>

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

enum struct FILESIZE_UNIT : uint32_t
{
    BYTE = 0,
    KB,
    MB,
    GB
};

std::tuple<std::string, FILESIZE_UNIT> filesize_to_string(float floating_point_val)
{
    int32_t file_size = floating_point_val;
    uint32_t digit_cnt = 0;

    /* convert the file size to B/KB/MB or GB */
    /*
        0 - Byte
        1 - Kilobyte
        2 - Megabyte
        3 - Gigabyte
     */
    FILESIZE_UNIT display_unit = FILESIZE_UNIT::BYTE;
    uint32_t display_unit_int = (uint32_t) display_unit;
    for (;; display_unit_int++)
    {
        if (floating_point_val > 1000)
        {
            floating_point_val /= 1000;
        }
        else
        {
            break;
        }
    }

    display_unit = (FILESIZE_UNIT) display_unit_int;

    /* for - count digits to be printed */
    for (uint32_t i = 1;; i *= 10)
    {
        if (display_unit == FILESIZE_UNIT::BYTE)
        {
            /* if - still bigger */
            if (file_size / i >= 1)
            {
                digit_cnt++;
                continue;
            }
            /* end if - still bigger */
        }
        else
        {
            /* if - still bigger */
            if (floating_point_val / i >= 1)
            {
                digit_cnt++;
                continue;
            }
            /* end if - still bigger */
        }
        break;
    }
    /* end for - count digits to be printed */
    char buffer[10];
    switch (display_unit)
    {
    case FILESIZE_UNIT::KB:
        sprintf(buffer, "%.1f KB", floating_point_val);
        break;

    case FILESIZE_UNIT::MB:
        sprintf(buffer, "%.1f MB", floating_point_val);
        break;

    case FILESIZE_UNIT::GB:
        sprintf(buffer, "%.1f GB", floating_point_val);
        break;

    default:
        sprintf(buffer, "%d B", file_size);
        break;
    }
    return {std::string(buffer), display_unit};
}

std::tuple<std::string, FILESIZE_UNIT> filesize_to_string(uint64_t file_size)
{
    float floating_point_val = file_size;
    uint32_t digit_cnt = 0;

    /* convert the file size to B/KB/MB or GB */
    /*
        0 - Byte
        1 - Kilobyte
        2 - Megabyte
        3 - Gigabyte
     */
    FILESIZE_UNIT display_unit = FILESIZE_UNIT::BYTE;
    uint32_t display_unit_int = (uint32_t) display_unit;
    for (;; display_unit_int++)
    {
        if (floating_point_val > 1000)
        {
            floating_point_val /= 1000;
        }
        else
        {
            break;
        }
    }

    display_unit = (FILESIZE_UNIT) display_unit_int;

    /* for - count digits to be printed */
    for (uint32_t i = 1;; i *= 10)
    {
        if (display_unit == FILESIZE_UNIT::BYTE)
        {
            /* if - still bigger */
            if (file_size / i >= 1)
            {
                digit_cnt++;
                continue;
            }
            /* end if - still bigger */
        }
        else
        {
            /* if - still bigger */
            if (floating_point_val / i >= 1)
            {
                digit_cnt++;
                continue;
            }
            /* end if - still bigger */
        }
        break;
    }
    /* end for - count digits to be printed */
    char buffer[10];
    switch (display_unit)
    {
    case FILESIZE_UNIT::KB:
        sprintf(buffer, "%.1f KB", floating_point_val);
        break;

    case FILESIZE_UNIT::MB:
        sprintf(buffer, "%.1f MB", floating_point_val);
        break;

    case FILESIZE_UNIT::GB:
        sprintf(buffer, "%.1f GB", floating_point_val);
        break;

    default:
        sprintf(buffer, "%lu B", file_size);
        break;
    }
    return {std::string(buffer), display_unit};
}

void init_colors_schemes()
{
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
}

void application::init()
{
    /* init logger */
    plog::init(plog::debug, "Log.txt");

    /* init ncurses */
    this->win = initscr();

    /* enable no-blocking getch */
    nodelay(win, TRUE);

    /* disable the cursor */
    curs_set(0);

    /* disable echoing input to stdout */
    noecho();

    /* initialize the color pairs */
    init_colors_schemes();

    input = init_input();

    /* get the content of the current and parent directory */
    current_dir = boost::filesystem::current_path();
    content_current_dir = fs::get_dir_content(current_dir, std::nullopt);
    content_parent_dir = fs::get_dir_content(current_dir.parent_path(), std::nullopt);
    selected_entry = boost::filesystem::path(content_current_dir[0]);
    content_child_dir = fs::get_dir_content(selected_entry, error);

    nlohmann::json config = load_config("config/config.json");

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
        0, 0, 10, 90,
        [&](const ui::component& self) -> void
        {
            uint32_t win_width = 0, win_height = 0;

            /* get window heigth and width */
            getmaxyx(win, win_height, win_width);

            /* if - no parent - abort */
            if (!current_dir.has_parent_path())
            {
                return;
            }
            /* end if - no parent - abort */

            /* move the cursor to the origin */
            const uint64_t origin_x = 0, origin_y = 0;
            cursor.move_abs(origin_x, origin_y);

            /* if - parent on top of parent tree */
            if (current_dir.parent_path() != content_parent_dir[0])
            {
                /* place parent path on top of parent tree */
                auto iter = std::find_if(content_parent_dir.begin(), content_parent_dir.end(),
                                         [&](const boost::filesystem::path& entry) -> bool
                                         { return entry == current_dir; });
                std::swap(content_parent_dir[0], *iter);
            }
            /* end if - parent on top of parent tree */

            uint32_t line_cnt = 0;

            /* for - iterate over content_parent_dir */
            for (std::vector<boost::filesystem::path>::iterator it = content_parent_dir.begin();
                 it < content_parent_dir.end(); it++)
            {
                /* if - line limit reached */
                if (line_cnt > win_height - 2)
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
                cursor.add_x(path_length);

                /* if - does the row have to filled */
                if (path_length < 28)
                {
                    /* for - file row */
                    for (size_t i = path_length; i < 28; i++)
                    {
                        addch(' ');
                        cursor.add_x(1);
                    }
                    /* end for - file row */
                }
                /* end if - does the row have to filled */

                line_cnt++;

                /* re-adjust the cursor */
                cursor.move_abs(origin_x, it - content_parent_dir.begin() + 1);
            }
            /* end for - iterate over content_parent_dir */
        });
    ui::component current_tree(
        10, 0, 40, 90,
        [&](const ui::component& self) -> void
        {
            uint32_t win_width = 0, win_height = 0;

            /* get window heigth and width */
            getmaxyx(win, win_height, win_width);

            /* move the cursor to the origin */
            const uint64_t origin_x = 30, origin_y = 0;
            cursor.move_abs(origin_x, origin_y);

            uint32_t line_cnt = 0;

            /* for - iterate over content_current_dir */
            for (std::vector<boost::filesystem::path>::iterator it = content_current_dir.begin();
                 it < content_current_dir.end(); it++)
            {
                /* if - line limit reached */
                if (line_cnt > win_height - 2)
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
                cursor.add_x(path_length);

                /* if - is entry directroy */
                if (boost::filesystem::is_directory(*it))
                {
                    uint32_t digit_cnt = 0;
                    uint32_t children_cnt = fs::get_children_count(*it, std::nullopt);

                    /* for - count digets to be printed */
                    for (uint32_t i = 1;; i *= 10)
                    {
                        /* if - still bigger */
                        if (children_cnt / i >= 1)
                        {
                            digit_cnt++;
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
                            if (28 - i > digit_cnt)
                            {
                                addch(' ');
                                cursor.add_x(1);
                            }
                            else
                            {
                                printw("%d", children_cnt);
                                cursor.add_x(digit_cnt);
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
                    uint64_t file_size = boost::filesystem::file_size(*it);
                    auto [file_size_str, unit] = filesize_to_string(file_size);

                    /* if - does the row have to filled */
                    if (path_length < 28)
                    {
                        /* for - print whitespaces */
                        for (size_t i = path_length; i < 28; i++)
                        {
                            /* if - print whitespace or child count */
                            if (28 - i > file_size_str.size())
                            {
                                addch(' ');
                                cursor.add_x(1);
                            }
                            else
                            {
                                printw("%s", file_size_str.c_str());
                                cursor.add_x(file_size_str.size());
                                break;
                            }
                            /* end if - print whitespace or child count */
                        }
                        /* end for - print whitespaces */
                    }
                    /* end if - does the row have to filled */
                }
                /* end if - is entry directroy */

                line_cnt++;

                /* re-adjust the cursor */
                cursor.move_abs(origin_x, 1 + it - content_current_dir.begin());
            }
            /* end for - iterate over content_current_dir */
        });
    ui::component preview_tab(
        50, 0, 50, 90,
        [&](const ui::component& self) -> void
        {
            uint32_t win_width = 0, win_height = 0;
            uint32_t line_cnt = 0;

            /* get window heigth and width */
            getmaxyx(win, win_height, win_width);

            /* move the cursor to the origin */
            const uint64_t origin_x = 60, origin_y = 0;
            cursor.move_abs(origin_x, origin_y);

            /* if - is a directory currently selected */
            if (directory_selected)
            {
                /* for - iterate over every entry in the selected directory */
                for (std::vector<boost::filesystem::path>::iterator it = content_child_dir.begin();
                     it < content_child_dir.end(); it++)
                {
                    /* if - line limit reached */
                    if (line_cnt > win_height - 2)
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
                    cursor.add_x(path_length);

                    /* if - does the row have to filled */
                    if (path_length < 28)
                    {
                        /* for - print whitespaces */
                        for (size_t i = path_length; i < 28; i++)
                        {
                            addch(' ');
                            cursor.add_x(1);
                        }
                        /* end for - print whitespaces */
                    }
                    /* end if - does the row have to filled */

                    line_cnt++;

                    /* re-adjust the cursor */
                    cursor.move_abs(origin_x, it - content_child_dir.begin() + 1);
                }
                /* end for - iterate over every entry in the selected directory */
            }
            else
            {
                uint64_t win_height = 0, win_width = 0, line_cnt = 0, char_line_cnt = 0;
                /* get the terminal size */
                getmaxyx(win, win_height, win_width);

                /* set the color mpde to default */
                attron(COLOR_PAIR(COLOR_SCHEME::DEFAULT_TEXT));

                /* for - iterate over every char in the preview */
                for (auto&& ch : file_preview)
                {
                    /* if - screen already full */
                    if (line_cnt > win_height - 2)
                    {
                        break;
                    }
                    /* end if - screen already full */

                    /* if - char is line break */
                    if (ch == '\n')
                    {
                        line_cnt++;
                        char_line_cnt = 0;
                        continue;
                    }
                    /* end if - char is line break */

                    /* if - gonna overflow terminal */
                    if (char_line_cnt > win_width - origin_x - 1)
                    {
                        line_cnt++;
                        char_line_cnt = 4;
                    }
                    /* end if - gonna overflow terminal */

                    /* re-adjust the cursor */
                    cursor.move_abs(60 + char_line_cnt, line_cnt);
                    addch(ch);
                    char_line_cnt++;
                    cursor.add_x(1);
                }
                /* end for - iterate over every char in the preview */
            }
            /* end if - is a directory currently selected */
        });
    ui::component bottom_bar(
        0, 0, 100, 10,
        [&](const ui::component& self) -> void
        {
            std::string descriptor = "";
            uint32_t win_width = 0, win_height = 0;

            /* get the terminal size */
            getmaxyx(win, win_height, win_width);

            /* move the cursor to the origin */
            const uint64_t origin_x = 0, origin_y = win_height - 1;
            cursor.move_abs(origin_x, origin_y);

            /*
                is_symblink has to called before because
                boost::filesystem::is_directory is also true on symlinks
            */
            /* if - directory selected & which type*/
            if (boost::filesystem::is_symlink(selected_entry))
            {
                descriptor.append("l");
            }
            else if (directory_selected) // Could also just call boost::filesystem::is_directory
            {
                descriptor.append("d");
            }
            else
            {
                descriptor.append("-");
            }
            /* end if - directory selected & which type*/

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

            /* get number of symbolic links or directories inside of directory */
            uint64_t child_sym_dir_cnt = 0;

            /* for - interate over every entry in selected entry */
            for (auto&& child : content_child_dir)
            {
                /* if - is child dir or sym */
                if (boost::filesystem::is_directory(child) || boost::filesystem::is_symlink(child))
                {
                    child_sym_dir_cnt++;
                }
                /* end if - is child dir or sym */
            }
            /* end for - interate over every entry in selected entry */

            descriptor.append(" " + std::to_string(child_sym_dir_cnt));

            /* get the owner and group passed in entry */
            auto [owner, group] = fs::get_dir_entry_group_owner(selected_entry, std::nullopt);
            descriptor.append(" " + owner + " " + group);

            /* if - directory selected & which type*/
            if (boost::filesystem::is_symlink(selected_entry))
            {
                descriptor.append(" " + boost::filesystem::read_symlink(selected_entry).native());
            }
            else
            {
                if (directory_selected)
                {
                    descriptor.append(" " + std::to_string(content_child_dir.size()));
                }
                else
                {
                    auto [file_size_str, unit] =
                        filesize_to_string(boost::filesystem::file_size(selected_entry));
                    descriptor.append(" " + file_size_str);
                }
            }
            /* end if - directory selected & which type*/

            /* append last time edited */
            char buffer[50] = {0};
            std::time_t last_write = boost::filesystem::last_write_time(selected_entry);
            struct tm* timeinfo = std::localtime(&last_write);
            strftime(buffer, 50, "%Y-%m-%d %H:%M", timeinfo);
            descriptor.append(" " + std::string(buffer));

            /* print descriptor */
            attron(COLOR_PAIR(COLOR_SCHEME::DEFAULT_TEXT));
            printw("%s", descriptor.c_str());
            cursor.add_x(descriptor.size());
        });

    /* create the component tree and add all components */
    ui_tree.add_comp(parent_tree);
    ui_tree.add_comp(current_tree);
    ui_tree.add_comp(preview_tab);
    ui_tree.add_comp(bottom_bar);
}

int32_t application::loop()
{
    bool should_close = false;

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

        /* if (window.is_resize())
            {
                ui_tree.update_resize();
            }
        */

        /* render the ui components */
        ui_tree.render();
    }
    /* end while - application loop */

    return EXIT_SUCCESS;
}

void application::deinit()
{
    /* de-init ncurses */
    endwin();
}