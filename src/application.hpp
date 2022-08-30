#pragma once

/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
#include <string>

/*<-------------- Application-Includes ---------------->*/
#include "fs_handler.hpp"
#include "cli.hpp"
#include "input.hpp"
#include "window.hpp"

/*<----------------- Library-Includes ----------------->*/
#if WIN32
#include <ncurses/ncurses.h>
#elif unix
#include <curses.h>
#endif
#include <boost/filesystem.hpp>

struct Application
{
  public:
    Application() = default;

    int32_t loop();
    void init();
    void deinit();

  private:
    Input input;
    UI::Window m_window;
    FS::Error error;
    std::vector<boost::filesystem::path> content_current_dir, content_parent_dir, content_child_dir;
    boost::filesystem::path selected_entry, parent_dir;
    boost::filesystem::path current_dir;
    std::string file_preview;
    uint64_t selected_entry_index = 0;
    bool directory_selected = true;
    UI::Cursor cursor = {0, 0};
    UI::ComponentTree ui_tree;
    uint32_t menu_selection_id, menu_settings_tabs_id, parent_tree_id, current_tree_id, preview_id,
        bottom_bar_id;
};