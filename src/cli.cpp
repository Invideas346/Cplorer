/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
/*<--------------- Application-Includes --------------->*/
#include "cli.hpp"
#include "ID_Creator.hpp"

/*<----------------- Library-Includes ----------------->*/
#include <plog/Log.h>

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

    component::component(uint8_t origin_x, uint8_t origin_y, uint8_t width, uint8_t height,
                         const render_func& func)
        : origin_x(origin_x), origin_y(origin_y), width(width), height(height),
          id(ID_CREATOR::create_id()), render_callback(func)

    {
    }
    void component::assign_render_routine(render_func fun) { this->render_callback = fun; }
    void component::render() const { this->render_callback(*this); }
    void component::update_resize(const window& win)
    {
        auto win_size = win.get_window_size();

        /* calcuate the start position */
        origin_render_coords.x = ((float) origin_x / 100) * win_size.width;
        origin_render_coords.y = ((float) origin_y / 100) * win_size.height;

        /* calculate the limit */
        limit_render_coords.x = (((float) width / 100) * win_size.width) + origin_render_coords.x;
        limit_render_coords.y = (((float) height / 100) * win_size.height) + origin_render_coords.y;
    }
    component::pos component::get_render_origin_coords() const { return origin_render_coords; }
    component::pos component::get_render_limit_coords() const { return limit_render_coords; }
    uint32_t component::get_id() const { return id; }
    bool component::is_active() const { return active; }
    void component::activate() { active = true; }
    void component::deactive() { active = false; }

    component_tree::component_tree(const std::vector<struct component>& components) {}
    component_tree& component_tree::add_comp(struct component& comp)
    {
        comps.push_back(comp);
        return *this;
    }
    component_tree& component_tree::remove_comp_index(uint32_t index)
    {
        if (index >= comps.size())
        {
            LOG_DEBUG << "Component at index: " + std::to_string(index) + " does not exist";
            return *this;
        }
        comps.erase(comps.begin() + index);
        return *this;
    }
    component_tree& component_tree::remove_comp_id(uint32_t id)
    {
        auto it =
            std::remove_if(comps.begin(), comps.end(),
                           [=](const component& component) { return component.get_id() == id; });
        if (it != comps.end())
        {
            LOG_DEBUG << "Component with ID " << std::to_string(id) << " not found!";
        }
        return *this;
    }
    component_tree& component_tree::disable_by_index(uint32_t index)
    {
        if (index >= comps.size())
        {
            LOG_DEBUG << "Component at index: " + std::to_string(index) + " does not exist";
            return *this;
        }
        comps.at(index).deactive();
        return *this;
    }
    component_tree& component_tree::disable_by_id(uint32_t id)
    {
        auto it =
            std::find_if(comps.begin(), comps.end(),
                         [=](const component& component) { return component.get_id() == id; });
        if (it != comps.end())
        {
            it->deactive();
        }
        return *this;
    }
    component_tree& component_tree::enable_by_index(uint32_t index)
    {
        if (index >= comps.size())
        {
            LOG_DEBUG << "Component at index: " + std::to_string(index) + " does not exist";
            return *this;
        }
        comps.at(index).activate();
        return *this;
    }
    component_tree& component_tree::enable_by_id(uint32_t id)
    {
        auto it =
            std::find_if(comps.begin(), comps.end(),
                         [=](const component& component) { return component.get_id() == id; });
        if (it != comps.end())
        {

            it->activate();
        }
        return *this;
    }
    void component_tree::pop_back() { comps.pop_back(); }
    void component_tree::pop_front() { comps.erase(comps.begin()); }
    void component_tree::render() const
    {
        for (auto& comp : comps)
        {
            if (comp.is_active())
            {
                comp.render();
            }
        }
    }
    void component_tree::update_resize(const window& win)
    {
        for (auto& comp : comps)
        {
            comp.update_resize(win);
        }
    }
} // namespace ui