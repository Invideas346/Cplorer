#pragma once

/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
#include <functional>

/*<-------------- Application-Includes ---------------->*/
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
        component() = default;
        component(uint8_t origin_x, uint8_t origin_y, uint8_t width, uint8_t height,
                  const render_func& func);

        void assign_render_routine(render_func fun);
        void render() const;
        void update_resize(const WINDOW* win);

        /* height, width, origin_x and origin_y are in percent */
        uint8_t width, height;
        uint8_t origin_x, origin_y;

      private:
        render_func render_callback;
    };

    struct component_tree
    {
      public:
        component_tree() = default;
        component_tree(const std::vector<struct component>& components);

        void add_comp(struct component& comp);
        void remove_comp(uint32_t index);
        void pop_back();
        void pop_front();

        void render() const;
        void update_resize(const WINDOW* win);

      private:
        std::vector<struct component> comps;
    };
} // namespace ui