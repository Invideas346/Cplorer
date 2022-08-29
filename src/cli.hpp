#pragma once

/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
#include <functional>

/*<-------------- Application-Includes ---------------->*/
#include "window.hpp"

/*<----------------- Library-Includes ----------------->*/
#if WIN32
#include <ncurses/ncurses.h>
#elif unix
#include <curses.h>
#endif

namespace ui
{
    struct cursor
    {
      public:
        cursor() = default;
        cursor(int32_t x, int32_t y);

        void set_x(int32_t x);
        void set_y(int32_t y);

        void add_x(int32_t x);
        void add_y(int32_t y);

        void move_rel(int32_t x, int32_t y);
        void move_abs(int32_t x, int32_t y);

        int32_t _x() const;
        int32_t _y() const;

      private:
        int32_t x, y;
    };

    /* forward declaration */
    struct component;
    typedef std::function<void(const ui::component&)> render_func;

    struct component
    {
      public:
        struct pos
        {
            uint32_t x, y;
        };

        component() = default;
        component(uint8_t origin_x, uint8_t origin_y, uint8_t width, uint8_t height,
                  const render_func& func);

        void assign_render_routine(render_func fun);
        void render() const;
        void update_resize(const window& win);

        pos get_render_origin_coords() const;
        pos get_render_limit_coords() const;

        uint32_t get_id() const;

        bool is_active() const;
        void activate();
        void deactive();

        /* height, width, origin_x and origin_y are in percent */
        uint8_t origin_x, origin_y;
        uint8_t width, height;

      private:
        bool active = true;
        uint32_t id;
        render_func render_callback;

        pos origin_render_coords;
        pos limit_render_coords;
    };

    struct component_tree
    {
      public:
        component_tree() = default;
        component_tree(const std::vector<struct component>& components);

        component_tree& add_comp(struct component& comp);
        component_tree& remove_comp_index(uint32_t index);
        component_tree& remove_comp_id(uint32_t id);

        component_tree& disable_by_index(uint32_t index);
        component_tree& disable_by_id(uint32_t id);

        component_tree& enable_by_index(uint32_t index);
        component_tree& enable_by_id(uint32_t id);

        void pop_back();
        void pop_front();

        void render() const;
        void update_resize(const window& win);

      private:
        std::vector<struct component> comps;
    };
} // namespace ui