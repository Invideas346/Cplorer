/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/

#if WIN32
#elif unix
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#endif

/*<--------------- Application-Includes --------------->*/
#include "fs_handler.hpp"

/*<----------------- Library-Includes ----------------->*/
#include <plog/Log.h>

namespace FS
{
    void Error::print() { fprintf(stderr, "%d %s", ec, message.c_str()); }

    static inline void SET_ERROR(std::optional<Error> error, Error::CODE code,
                                 const std::string& message = "")
    {
        if (error)
        {
            error->ec = code;
            error->message = std::move(message);
        }
    }

    static inline uint64_t get_children_count_local(const boost::filesystem::path& path,
                                                    std::optional<Error> error)
    {
        /* if - is passed in path a directory */
        if (boost::filesystem::is_directory(path) == false)
        {
            LOG_DEBUG << "Passed in path is not a directory " + std::string(path.native().c_str());
            SET_ERROR(error, Error::PATH_NOT_DIR, "Passed in path is not a directory");
            return 0;
        }
        /* end if - is passed in path a directory */

        uint64_t count = 0;

        /* get a directory iterator over the directory */
        auto dir_iter = boost::filesystem::directory_iterator(path);

        /* for - iterate over all directory entries */
        for (auto&& dir_entry : dir_iter)
        {
            count++;
        }
        /* end for - iterate over all directory entries */

        SET_ERROR(error, Error::NO_ERROR);
        return count;
    }

    static inline std::vector<boost::filesystem::path>
    get_dir_content_local(const boost::filesystem::path& path, std::optional<Error> error)
    {
        std::vector<boost::filesystem::path> m_children;

        /* if - is passed in path a directory */
        if (boost::filesystem::is_directory(path) == false)
        {
            LOG_DEBUG << "Passed in path is not a directory " + std::string(path.native().c_str());
            SET_ERROR(error, Error::INVALID_ARGUMENT, "Passed in path is not a directory");
            return {};
        }
        /* end if - is passed in path a directory */

        /* get a directory iterator over the directory */
        auto dir_iter = boost::filesystem::directory_iterator(path);

        /* for - iterate over all director entries */
        for (auto&& entry : dir_iter)
        {
            m_children.push_back(entry.path());
        }
        /* end for - iterate over all director entries */

        SET_ERROR(error, Error::NO_ERROR);
        m_children = sort_paths(m_children);
        return m_children;
    }

    static inline std::string get_file_content_local(const boost::filesystem::path& path,
                                                     std::optional<Error> error)
    {
        /* if - path pointing to regular file */
        if (boost::filesystem::is_regular_file(path) == false)
        {
            LOG_DEBUG << "Passed in path is not a file " + std::string(path.native().c_str());
            SET_ERROR(error, Error::INVALID_ARGUMENT, "Passed in path is not a file");
            return "";
        }
        /* end if - path pointing to regular file */

        /* allocate enough memory */
        std::string data;
        data.reserve(boost::filesystem::file_size(path));
        std::fstream fs{path};

        /* if - check if the file stream could open */
        if (fs.is_open() == false)
        {
            LOG_DEBUG << "Could not open filestream " + std::string(path.native().c_str());
            SET_ERROR(error, Error::GENERAL_ERROR, "Could not open filestream");
            return "";
        }
        /* end if - check if the file stream could open */

        constexpr uint64_t CHUNK_SIZE = 4096;
        char buffer[CHUNK_SIZE] = {0};

        /* while - read till eof is reached */
        while (fs.eof() == false)
        {
            fs.read(buffer, CHUNK_SIZE);
        }
        /* end while - read till eof is reached */

        data = buffer;
        SET_ERROR(error, Error::NO_ERROR);
        return data;
    }

    static inline std::string get_file_content_n_local(const boost::filesystem::path& path,
                                                       uint64_t n, std::optional<Error> error)
    {
        /* if - path pointing to regular file */
        if (boost::filesystem::is_regular_file(path) == false)
        {
            LOG_DEBUG << "Passed in path is not a file " << std::string(path.native().c_str());
            SET_ERROR(error, Error::INVALID_ARGUMENT, "Passed in path is not a file");
            return "";
        }
        /* end if - path pointing to regular file */

        std::fstream fs{path};

        /* if - check if the file stream could open */
        if (fs.is_open() == false)
        {
            LOG_DEBUG << "Could not open filestream " << std::string(path.native().c_str());
            SET_ERROR(error, Error::GENERAL_ERROR, "Could not open filestream");
            return "";
        }
        /* end if - check if the file stream could open */

        /* how many bytes schould be read */
        uint64_t left_file_size = boost::filesystem::file_size(path);
        uint64_t to_read_bytes = (left_file_size < n) ? left_file_size : n;

        /* allocate enough memory */
        std::string data;
        data.reserve(to_read_bytes + 1);
        constexpr uint64_t CHUNK_SIZE = 4096;
        char* buffer = new char[to_read_bytes + 1];
        uint64_t chunk_cntr = 0;

        /* TODO: refactoring this mess */
        if (left_file_size < to_read_bytes || left_file_size < CHUNK_SIZE)
        {
            (to_read_bytes < left_file_size) ? fs.read(buffer, to_read_bytes)
                                             : fs.read(buffer, left_file_size);
        }
        else
        {
            /* while - read till eof is reached */
            while (fs.eof() == false && CHUNK_SIZE * (chunk_cntr + 1) < to_read_bytes)
            {
                fs.read(buffer + chunk_cntr * CHUNK_SIZE,
                        to_read_bytes < CHUNK_SIZE ? to_read_bytes : CHUNK_SIZE);
                chunk_cntr++;
            }
            /* end while - read till eof is reached */
            fs.read(buffer + chunk_cntr * CHUNK_SIZE, to_read_bytes - chunk_cntr * CHUNK_SIZE);
        }

        fs.close();
        buffer[to_read_bytes] = '\0';
        data = buffer;
        delete[] buffer;
        SET_ERROR(error, Error::NO_ERROR);
        return data;
    }
#if WIN32
    std::tuple<std::string> get_dir_entry_group_owner_local(const boost::filesystem::path& path,
                                                            std::optional<Error> error)
    {
    }
#elif unix
    static inline std::tuple<std::string, std::string>
    get_dir_entry_group_owner_local(const boost::filesystem::path& path, std::optional<Error> error)
    {
        if (boost::filesystem::exists(path) == false)
        {
            LOG_DEBUG << "Path does not point to file or directory "
                      << std::string(path.native().c_str());
            SET_ERROR(error, Error::PATH_NOT_EXISTS,
                      "path does not point to file or directory" +
                          std::string(path.native().c_str()));
            return {"", ""};
        }
        struct stat info;
        stat(path.native().c_str(), &info);
        struct passwd* pw = getpwuid(info.st_uid);
        struct group* gr = getgrgid(info.st_gid);
        return {pw->pw_name, gr->gr_name};
    }
#endif

    std::vector<boost::filesystem::path> sort_paths(std::vector<boost::filesystem::path> paths)
    {
        std::vector<boost::filesystem::path> sorted_vector;
        /* for - iterate over all children */
        for (uint32_t i = 0; i < paths.size(); i++)
        {
            /* if - is entry a directory */
            if (boost::filesystem::is_directory(paths[i]))
            {
                sorted_vector.push_back(paths[i]);
            }
            /* end if - is entry a directory */
        }
        /* end for - iterate over all children */

        /* for - iterate over all children */
        for (uint32_t i = 0; i < paths.size(); i++)
        {
            /* if - is entry a directory */
            if (boost::filesystem::is_directory(paths[i]) == false)
            {
                sorted_vector.push_back(paths[i]);
            }
            /* end if - is entry a directory */
        }
        /* end for - iterate over all children */

        return sorted_vector;
    }

    std::vector<boost::filesystem::path> get_dir_content(const char* path,
                                                         std::optional<Error> error)
    {
        return get_dir_content_local(boost::filesystem::path(path), error);
    }

    std::vector<boost::filesystem::path> get_dir_content(const std::string& path,
                                                         std::optional<Error> error)
    {
        return get_dir_content_local(boost::filesystem::path(path), error);
    }

    std::vector<boost::filesystem::path> get_dir_content(const boost::filesystem::path& path,
                                                         std::optional<Error> error)
    {
        return get_dir_content_local(path, error);
    }

    std::string get_file_content(const char* path, std::optional<Error> error)
    {
        return get_file_content_local(boost::filesystem::path(path), error);
    }

    std::string get_file_content(const std::string& path, std::optional<Error> error)
    {
        return get_file_content_local(boost::filesystem::path(path), error);
    }

    std::string get_file_content(const boost::filesystem::path& path, std::optional<Error> error)
    {
        return get_file_content_local(path, error);
    }

    std::string get_file_content_n(const char* path, uint64_t n, std::optional<Error> error)
    {
        return get_file_content_n(std::string(path), n, error);
    }

    std::string get_file_content_n(const std::string& path, uint64_t n, std::optional<Error> error)
    {
        return get_file_content_n_local(boost::filesystem::path(path), n, error);
    }

    std::string get_file_content_n(const boost::filesystem::path& path, uint64_t n,
                                   std::optional<Error> error)
    {
        return get_file_content_n_local(path, n, error);
    }

    uint64_t get_children_count(const char* path, std::optional<Error> error)
    {
        return get_children_count_local(boost::filesystem::path(path), error);
    }

    uint64_t get_children_count(const std::string& path, std::optional<Error> error)
    {
        return get_children_count_local(boost::filesystem::path(path), error);
    }

    uint64_t get_children_count(const boost::filesystem::path& path, std::optional<Error> error)
    {
        return get_children_count_local(path, error);
    }

    std::tuple<std::string, std::string> get_dir_entry_group_owner(const char* path,
                                                                   std::optional<Error> error)
    {
        return get_dir_entry_group_owner_local(boost::filesystem::path(path), error);
    }

    std::tuple<std::string, std::string> get_dir_entry_group_owner(const std::string& path,
                                                                   std::optional<Error> error)
    {
        return get_dir_entry_group_owner_local(boost::filesystem::path(path), error);
    }

    std::tuple<std::string, std::string>
    get_dir_entry_group_owner(const boost::filesystem::path& path, std::optional<Error> error)
    {
        return get_dir_entry_group_owner_local(path, error);
    }
} // namespace FS