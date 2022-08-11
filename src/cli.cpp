#include "cli.hpp"

namespace ui
{
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