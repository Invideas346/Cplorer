#pragma once

#include <vector>
#include <string>
#include <boost/filesystem.hpp>

namespace fs
{
    enum class ERROR_CODE
    {
        NO_ERROR = 0x0,
        GENERAL_ERROR = 0x1,
        INVALID_ARGUMENT = 0x2 | GENERAL_ERROR
    };

    std::vector<std::string> get_dir_content(const std::string& path, ERROR_CODE* ec = nullptr);
    std::vector<std::string> get_dir_content(const boost::filesystem::path& path,
                                             ERROR_CODE* ec = nullptr);

    std::string get_file_content(const std::string& path, ERROR_CODE* ec = nullptr);
    std::string get_file_content(const boost::filesystem::path& path, ERROR_CODE* ec = nullptr);
} // namespace fs
