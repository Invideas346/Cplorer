
/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
/*<-------------- Application-Includes ---------------->*/
#include "config.hpp"
#include "fs_handler.hpp"

/*<----------------- Library-Includes ----------------->*/

nlohmann::json load_config(const char* path)
{
    nlohmann::json obj = nlohmann::json::parse(FS::get_file_content(path, std::nullopt));
    return (obj.is_discarded()) ? nullptr : obj;
}

nlohmann::json load_config(const std::string& path)
{
    nlohmann::json obj = nlohmann::json::parse(FS::get_file_content(path, std::nullopt));
    return (obj.is_discarded()) ? nullptr : obj;
}

nlohmann::json load_config(const boost::filesystem::path& path)
{
    nlohmann::json obj = nlohmann::json::parse(FS::get_file_content(path, std::nullopt));
    return (obj.is_discarded()) ? nullptr : obj;
}