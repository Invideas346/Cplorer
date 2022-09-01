
/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
/*<-------------- Application-Includes ---------------->*/
#include "config.hpp"
#include "fs_handler.hpp"

/*<----------------- Library-Includes ----------------->*/

nlohmann::json load_config(const char* path)
{
    if (boost::filesystem::exists(boost::filesystem::path(path)))
    {

        nlohmann::json obj = nlohmann::json::parse(FS::get_file_content(path, std::nullopt));
        return (obj.is_discarded()) ? nullptr : obj;
    }
    return nullptr;
}

nlohmann::json load_config(const std::string& path)
{
    if (boost::filesystem::exists(boost::filesystem::path(path)))
    {

        nlohmann::json obj = nlohmann::json::parse(FS::get_file_content(path, std::nullopt));
        return (obj.is_discarded()) ? nullptr : obj;
    }
    return nullptr;
}

nlohmann::json load_config(const boost::filesystem::path& path)
{
    if (boost::filesystem::exists(boost::filesystem::path(path)))
    {

        nlohmann::json obj = nlohmann::json::parse(FS::get_file_content(path, std::nullopt));
        return (obj.is_discarded()) ? nullptr : obj;
    }
    return nullptr;
    nlohmann::json obj = nlohmann::json::parse(FS::get_file_content(path, std::nullopt));
}