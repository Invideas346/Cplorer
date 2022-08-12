#include "fs_handler.hpp"

namespace fs
{
    void error::print() { fprintf(stderr, "%d %s", ec, message.c_str()); }

    static inline void SET_ERROR(error* error, error::CODE code, const std::string& message = "")
    {
        if (error != nullptr)
        {
            error->ec = code;
            error->message = std::move(message);
        }
    }

    std::vector<std::string> get_dir_content(const char* path, error* error)
    {
        return get_dir_content(std::string(path), error);
    }

    std::vector<std::string> get_dir_content(const std::string& path, error* error)
    {
        std::vector<std::string> m_children;
        auto dir_path = boost::filesystem::path(path);

        /* if - is passed in path a directory */
        if (boost::filesystem::is_directory(dir_path) == false)
        {
            SET_ERROR(error, error::INVALID_ARGUMENT, "Passed in path is not a directory");
            return {};
        }
        /* end if - is passed in path a directory */

        /* get a directory iterator over the directory */
        auto dir_iter = boost::filesystem::directory_iterator(dir_path);

        /* for - iterate over all director entries */
        for (auto&& dir_entry : dir_iter)
        {
            m_children.push_back(dir_entry.path().relative_path().native().c_str());
        }
        /* end for - iterate over all director entries */

        SET_ERROR(error, error::NO_ERROR);
        return m_children;
    }

    std::vector<std::string> get_dir_content(const boost::filesystem::path& path, error* error)
    {
        std::vector<std::string> m_children;
        std::string path_str = path.native().c_str();

        /* if - is passed in path a directory */
        if (!boost::filesystem::is_directory(path))
        {
            SET_ERROR(error, error::INVALID_ARGUMENT, "Passed in path is not a directory");
            return {};
        }
        /* end if - is passed in path a directory */

        /* get a directory iterator over the directory */
        auto dir_iter = boost::filesystem::directory_iterator(path);

        /* for - iterate over all director entries */
        for (auto&& entry : dir_iter)
        {
            m_children.push_back(entry.path().relative_path().native().c_str());
        }
        /* end for - iterate over all director entries */

        SET_ERROR(error, error::NO_ERROR);
        return m_children;
    }

    std::string get_file_content(const char* path, error* error)
    {
        return get_file_content(std::string(path), error);
    }

    std::string get_file_content(const std::string& path, error* error)
    {
        /* if - path pointing to regular file */
        if (!boost::filesystem::is_regular_file(path))
        {
            SET_ERROR(error, error::INVALID_ARGUMENT, "Passed in path is not a file");
            return "";
        }
        /* end if - path pointing to regular file */

        /* allocate enough memory */
        std::string data;
        data.reserve(boost::filesystem::file_size(path));
        boost::filesystem::fstream fs{path};

        /* if - check if the file stream could open */
        if (!fs.is_open())
        {
            SET_ERROR(error, error::GENERAL_ERROR, "Could not open filestream");
            return "";
        }
        /* end if - check if the file stream could open */

        constexpr uint64_t CHUNK_SIZE = 4096;
        char buffer[CHUNK_SIZE] = {0};

        /* while - read till eof is reached */
        while (!fs.eof())
        {
            fs.read(buffer, CHUNK_SIZE);
        }
        /* while - read till eof is reached */

        data = buffer;
        SET_ERROR(error, error::NO_ERROR);
        return data;
    }

    std::string get_file_content(const boost::filesystem::path& path, error* error)
    {
        /* if - path pointing to regular file */
        if (!boost::filesystem::is_regular_file(path))
        {
            SET_ERROR(error, error::INVALID_ARGUMENT, "Passed in path is not a file");
            return "";
        }
        /* end if - path pointing to regular file */

        /* allocate enough memory */
        std::string data;
        data.reserve(boost::filesystem::file_size(path));
        boost::filesystem::fstream fs{path};

        /* if - check if the file stream could open */
        if (!fs.is_open())
        {
            SET_ERROR(error, error::GENERAL_ERROR, "Could not open filestream");
            return "";
        }
        /* end if - check if the file stream could open */

        constexpr uint64_t CHUNK_SIZE = 4096;
        char buffer[CHUNK_SIZE] = {0};

        /* while - read till eof is reached */
        while (fs.tellg() != fs.eof())
        {
            fs.read(buffer, CHUNK_SIZE);
        }
        /* while - read till eof is reached */

        SET_ERROR(error, error::NO_ERROR);
        return data;
    }

    std::string get_file_content_n(const char* path, uint64_t n, error* error)
    {
        return get_file_content_n(std::string(path), n, error);
    }

    std::string get_file_content_n(const std::string& path, uint64_t n, error* error)
    {
        /* if - path pointing to regular file */
        if (!boost::filesystem::is_regular_file(path))
        {
            SET_ERROR(error, error::INVALID_ARGUMENT, "Passed in path is not a file");
            return "";
        }
        /* end if - path pointing to regular file */

        /* allocate enough memory */
        std::string data;
        data.reserve(n);
        boost::filesystem::ifstream fs{path};

        /* if - check if the file stream could open */
        if (!fs.is_open())
        {
            SET_ERROR(error, error::GENERAL_ERROR, "Could not open filestream");
            return "";
        }
        /* end if - check if the file stream could open */

        constexpr uint64_t CHUNK_SIZE = 4096;
        char* buffer = new char[n];
        uint64_t chunk_cntr = 0;

        /* while - read till eof is reached */
        while (!fs.eof() && CHUNK_SIZE * chunk_cntr < n)
        {
            fs.read(buffer + chunk_cntr, n < CHUNK_SIZE ? n : CHUNK_SIZE);
            chunk_cntr++;
        }
        /* while - read till eof is reached */

        data = buffer;
        free(buffer);
        SET_ERROR(error, error::NO_ERROR);
        return data;
    }

    std::string get_file_content_n(const boost::filesystem::path& path, uint64_t n, error* error)
    {
        /* if - path pointing to regular file */
        if (!boost::filesystem::is_regular_file(path))
        {
            SET_ERROR(error, error::INVALID_ARGUMENT, "Passed in path is not a file");
            return "";
        }
        /* end if - path pointing to regular file */

        /* allocate enough memory */
        std::string data;
        data.reserve(n);
        boost::filesystem::ifstream fs{path};

        /* if - check if the file stream could open */
        if (!fs.is_open())
        {
            SET_ERROR(error, error::GENERAL_ERROR, "Could not open filestream");
            return "";
        }
        /* end if - check if the file stream could open */

        constexpr uint64_t CHUNK_SIZE = 4096;
        char buffer[CHUNK_SIZE] = {0};

        /* while - read till eof is reached */
        while (fs.tellg() != fs.eof())
        {
            fs.read(buffer, CHUNK_SIZE);
        }
        /* while - read till eof is reached */

        SET_ERROR(error, error::NO_ERROR);
        return data;
    }
} // namespace fs