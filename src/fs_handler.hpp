#pragma once

#include <vector>
#include <string>
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

    std::vector<std::string> get_dir_content(const char* path, error* error = nullptr);
    std::vector<std::string> get_dir_content(const std::string& path, error* error = nullptr);
    std::vector<std::string> get_dir_content(const boost::filesystem::path& path,
                                             error* error = nullptr);

    std::string get_file_content(const char* path, error* error = nullptr);
    std::string get_file_content(const std::string& path, error* error = nullptr);
    std::string get_file_content(const boost::filesystem::path& path, error* error = nullptr);

    std::string get_file_content_n(const char* path, uint64_t n, error* error = nullptr);
    std::string get_file_content_n(const std::string& path, uint64_t n, error* error = nullptr);
    std::string get_file_content_n(const boost::filesystem::path& path, uint64_t n,
                                   error* error = nullptr);
} // namespace fs
