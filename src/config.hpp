#pragma once 

/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
#include <string>

/*<-------------- Application-Includes ---------------->*/
/*<----------------- Library-Includes ----------------->*/
#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>

nlohmann::json load_config(const char* path);
nlohmann::json load_config(const std::string& path);
nlohmann::json load_config(const boost::filesystem::path& path);