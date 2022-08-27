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

struct application
{
  public:
    application() = default;

    int32_t loop();
    void init();
    void deinit();

  private:
    input input;
    ui::window m_window;
    fs::error error;
    std::vector<boost::filesystem::path> content_current_dir, content_parent_dir, content_child_dir;
    boost::filesystem::path selected_entry, parent_dir;
    boost::filesystem::path current_dir;
    std::string file_preview;
    uint64_t selected_entry_index = 0;
    bool directory_selected = true;
    ui::cursor cursor = {0, 0};
    ui::component_tree ui_tree;
};