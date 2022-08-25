/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
/*<--------------- Application-Includes --------------->*/
#include "cli.hpp"

/*<----------------- Library-Includes ----------------->*/

namespace ui
{
    cursor::cursor(int32_t x, int32_t y) : x(x), y(y) {}

    void cursor::move_rel(int32_t x, int32_t y)
    {
        this->x += x;
        this->y += y;
        move(this->y, this->x);
    }

    void cursor::move_abs(int32_t x, int32_t y)
    {
        this->x = x;
        this->y = y;
        move(this->y, this->x);
    }

    int32_t cursor::_x() const { return x; }
    int32_t cursor::_y() const { return y; }

    void cursor::set_x(int32_t x) { this->x = x; }
    void cursor::set_y(int32_t y) { this->y = y; }

    void cursor::add_x(int32_t x) { this->x += x; }
    void cursor::add_y(int32_t y) { this->y += y; }

    component::component(const render_func& func) : render_callback(func) {}

    void component::assign_render_routine(render_func fun) { this->render_callback = fun; }

    void component::render() const { this->render_callback(); }

    component_tree::component_tree(const std::vector<struct component>& components) {}

    void component_tree::add_comp(struct component& comp) { comps.push_back(comp); }

    void component_tree::remove_comp(uint32_t index) { comps.erase(comps.begin() + index); }

    void component_tree::pop_back() { comps.pop_back(); }

    void component_tree::pop_front() { comps.erase(comps.begin()); }

    void component_tree::render() const
    {
        for (auto& comp : comps)
        {
            comp.render();
        }
    }
} // namespace ui