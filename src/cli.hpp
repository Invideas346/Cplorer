#pragma once

#if WIN32
#include <ncurses/ncurses.h>
#elif unix
#include <curses.h>
#endif

#include <functional>

typedef std::function<void()> render_func;

namespace ui
{
    struct component
    {
      public:
        component() = default;
        component(const render_func& fun);

        void assign_render_routine(render_func fun);
        void render() const;

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

      private:
        std::vector<struct component> comps;
    };
} // namespace ui