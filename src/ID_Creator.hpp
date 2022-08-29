#pragma once

/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
#include <cstdint>
#include <vector>
#include <algorithm>
#include <string>

/*<-------------- Application-Includes ---------------->*/
/*<----------------- Library-Includes ----------------->*/
#include <plog/Log.h>

struct ID_CREATOR
{
  public:
    static uint64_t create_id()
    {
        if (free_ids.empty())
        {
            return id_counter++;
        }
        uint64_t last_value = free_ids.back();
        free_ids.pop_back();
        return last_value;
    }
    static void free_id(uint64_t id)
    {
        auto it = std::find_if(free_ids.begin(), free_ids.end(),
                               [=](const uint64_t _id) -> bool { return _id == id; });
        if (it == free_ids.end())
        {
            LOG_DEBUG << "ID " << std::to_string(id) << " does not exist";
            return;
        }
        free_ids.push_back(*it);
    }

  private:
    static uint64_t id_counter;
    static std::vector<uint64_t> free_ids;
};

uint64_t ID_CREATOR::id_counter = 0;
std::vector<uint64_t> ID_CREATOR::free_ids = {};