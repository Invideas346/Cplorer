#pragma once

/*<================= Include =================>*/
/*<----------------- System ------------------>*/
#include <vector>
#include <string>
#include <optional>

/*<----------------- Library ----------------->*/
#include <boost/filesystem.hpp>

namespace fs
{
    struct error
    {
        enum CODE
        {
            NO_ERROR = 0x0,
            GENERAL_ERROR = 0x1,
            INVALID_ARGUMENT = 0x2 | GENERAL_ERROR,
        };

        void print();

        std::string message = "";
        CODE ec = NO_ERROR;
    };

    std::vector<boost::filesystem::path> get_dir_content(const char* path,
                                                         std::optional<error> error);
    std::vector<boost::filesystem::path> get_dir_content(const std::string& path,
                                                         std::optional<error> error);
    std::vector<boost::filesystem::path> get_dir_content(const boost::filesystem::path& path,
                                                         std::optional<error> error);

    std::string get_file_content(const char* path, std::optional<error> error);
    std::string get_file_content(const std::string& path, std::optional<error> error);
    std::string get_file_content(const boost::filesystem::path& path, std::optional<error> error);

    std::string get_file_content_n(const char* path, uint64_t n, std::optional<error> error);
    std::string get_file_content_n(const std::string& path, uint64_t n, std::optional<error> error);
    std::string get_file_content_n(const boost::filesystem::path& path, uint64_t n,
                                   std::optional<error> error);
} // namespace fs
