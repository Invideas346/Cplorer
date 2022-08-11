#include "cli.hpp"

namespace ui
{
    component::component(const render_func& func) : render_callback(func) {}

    void component::attach_render_routine(render_func fun) { this->render_callback = fun; }

    void component::render() { this->render_callback(); }

    component_tree::component_tree(const std::vector<struct component>& components) {}

    void component_tree::add_comp(struct component& comp) {}

    void component_tree::remove_comp(uint32_t index) {}

    void component_tree::pop_back() {}

    void component_tree::pop_front() {}

    void component_tree::render()
    {
        for (const auto& comp : comps)
        {
            comp.render();
        }
    }
} // namespace ui