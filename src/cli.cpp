/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
/*<--------------- Application-Includes --------------->*/
#include "cli.hpp"
#include "ID_Creator.hpp"

/*<----------------- Library-Includes ----------------->*/
#include <plog/Log.h>

namespace UI
{

    Cursor::Cursor(int32_t x, int32_t y) : x(x), y(y) {}
    void Cursor::move_rel(int32_t x, int32_t y)
    {
        this->x += x;
        this->y += y;
        move(this->y, this->x);
    }
    void Cursor::move_abs(int32_t x, int32_t y)
    {
        this->x = x;
        this->y = y;
        move(this->y, this->x);
    }
    int32_t Cursor::_x() const { return x; }
    int32_t Cursor::_y() const { return y; }
    void Cursor::set_x(int32_t x) { this->x = x; }
    void Cursor::set_y(int32_t y) { this->y = y; }
    void Cursor::add_x(int32_t x) { this->x += x; }
    void Cursor::add_y(int32_t y) { this->y += y; }

    Component::Component(uint8_t origin_x, uint8_t origin_y, uint8_t width, uint8_t height,
                         const render_func& func)
        : origin_x(origin_x), origin_y(origin_y), width(width), height(height),
          id(ID_CREATOR::create_id()), render_callback(func)

    {
    }
    void Component::assign_render_routine(render_func fun) { this->render_callback = fun; }
    void Component::render() const { this->render_callback(*this); }
    void Component::update_resize(const Window& win)
    {
        auto win_size = win.get_window_size();

        /* calcuate the start position */
        origin_render_coords.x = ((float) origin_x / 100) * win_size.width;
        origin_render_coords.y = ((float) origin_y / 100) * win_size.height;

        /* calculate the limit */
        limit_render_coords.x = (((float) width / 100) * win_size.width) + origin_render_coords.x;
        limit_render_coords.y = (((float) height / 100) * win_size.height) + origin_render_coords.y;
    }
    Component::pos Component::get_render_origin_coords() const { return origin_render_coords; }
    Component::pos Component::get_render_limit_coords() const { return limit_render_coords; }
    uint32_t Component::get_id() const { return id; }
    bool Component::is_active() const { return active; }
    void Component::activate() { active = true; }
    void Component::deactive() { active = false; }

    ComponentTree::ComponentTree(const std::vector<struct Component>& components) {}
    ComponentTree& ComponentTree::add_comp(struct Component& comp)
    {
        comps.push_back(comp);
        return *this;
    }
    ComponentTree& ComponentTree::remove_comp_index(uint32_t index)
    {
        if (index >= comps.size())
        {
            LOG_DEBUG << "Component at index: " + std::to_string(index) + " does not exist";
            return *this;
        }
        comps.erase(comps.begin() + index);
        return *this;
    }
    ComponentTree& ComponentTree::remove_comp_id(uint32_t id)
    {
        auto it = std::remove_if(comps.begin(), comps.end(), [=](const Component& component) {
            return component.get_id() == id;
        });
        if (it != comps.end())
        {
            LOG_DEBUG << "Component with ID " << std::to_string(id) << " not found!";
        }
        return *this;
    }
    ComponentTree& ComponentTree::disable_by_index(uint32_t index)
    {
        if (index >= comps.size())
        {
            LOG_DEBUG << "Component at index: " + std::to_string(index) + " does not exist";
            return *this;
        }
        comps.at(index).deactive();
        return *this;
    }
    ComponentTree& ComponentTree::disable_by_id(uint32_t id)
    {
        auto it = std::find_if(comps.begin(), comps.end(), [=](const Component& component) {
            return component.get_id() == id;
        });
        if (it != comps.end())
        {
            it->deactive();
        }
        return *this;
    }
    ComponentTree& ComponentTree::enable_by_index(uint32_t index)
    {
        if (index >= comps.size())
        {
            LOG_DEBUG << "Component at index: " + std::to_string(index) + " does not exist";
            return *this;
        }
        comps.at(index).activate();
        return *this;
    }
    ComponentTree& ComponentTree::enable_by_id(uint32_t id)
    {
        auto it = std::find_if(comps.begin(), comps.end(), [=](const Component& component) {
            return component.get_id() == id;
        });
        if (it != comps.end())
        {

            it->activate();
        }
        return *this;
    }
    void ComponentTree::pop_back() { comps.pop_back(); }
    void ComponentTree::pop_front() { comps.erase(comps.begin()); }
    void ComponentTree::render() const
    {
        for (auto& comp : comps)
        {
            if (comp.is_active())
            {
                comp.render();
            }
        }
    }
    void ComponentTree::update_resize(const Window& win)
    {
        for (auto& comp : comps)
        {
            comp.update_resize(win);
        }
    }
} // namespace UI