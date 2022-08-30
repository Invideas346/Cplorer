#pragma once

/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
#include <vector>
#include <string>
#include <optional>
#include <fstream>
#include <tuple>

/*<-------------- Application-Includes ---------------->*/
/*<----------------- Library-Includes ----------------->*/
#include <boost/filesystem.hpp>

namespace FS
{
    struct Error
    {
        enum CODE
        {
            NO_ERROR = 0x0,
            GENERAL_ERROR = 0x1,
            INVALID_ARGUMENT = 0x2 | GENERAL_ERROR,
            PATH_NOT_DIR = 0x04 | GENERAL_ERROR | INVALID_ARGUMENT,
            PATH_NOT_EXISTS = 0x8 | GENERAL_ERROR | INVALID_ARGUMENT
        };

        void print();

        std::string message = "";
        CODE ec = NO_ERROR;
    };

    std::vector<boost::filesystem::path> sort_paths(std::vector<boost::filesystem::path> paths);

    std::vector<boost::filesystem::path> get_dir_content(const char* path,
                                                         std::optional<Error> error);
    std::vector<boost::filesystem::path> get_dir_content(const std::string& path,
                                                         std::optional<Error> error);
    std::vector<boost::filesystem::path> get_dir_content(const boost::filesystem::path& path,
                                                         std::optional<Error> error);

    std::string get_file_content(const char* path, std::optional<Error> error);
    std::string get_file_content(const std::string& path, std::optional<Error> error);
    std::string get_file_content(const boost::filesystem::path& path, std::optional<Error> error);

    std::string get_file_content_n(const char* path, uint64_t n, std::optional<Error> error);
    std::string get_file_content_n(const std::string& path, uint64_t n, std::optional<Error> error);
    std::string get_file_content_n(const boost::filesystem::path& path, uint64_t n,
                                   std::optional<Error> error);

    uint64_t get_children_count(const char* path, std::optional<Error> error);
    uint64_t get_children_count(const std::string& path, std::optional<Error> error);
    uint64_t get_children_count(const boost::filesystem::path& path, std::optional<Error> error);

    std::tuple<std::string, std::string> get_dir_entry_group_owner(const char* path,
                                                                   std::optional<Error> error);
    std::tuple<std::string, std::string> get_dir_entry_group_owner(const std::string& path,
                                                                   std::optional<Error> error);
    std::tuple<std::string, std::string>
    get_dir_entry_group_owner(const boost::filesystem::path& path, std::optional<Error> error);
} // namespace FS
