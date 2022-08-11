#include "fs_handler.hpp"

namespace fs {
    static inline void SET_ERROR_CODE(ERROR_CODE* ec, ERROR_CODE code) { if(ec != nullptr) *ec = code; }

    std::vector<std::string> get_dir_content(const std::string& path, ERROR_CODE* ec)
    {
        std::vector<std::string> m_children;
        auto dir_path = boost::filesystem::path(path);
        
        /* if - is passed in path a directory */
        if(boost::filesystem::is_directory(dir_path) == false)
        {
            SET_ERROR_CODE(ec, ERROR_CODE::INVALID_ARGUMENT);
            return {};
        }
        /* end if - is passed in path a directory */

        /* get a directory iterator over the directory */
        auto dir_iter = boost::filesystem::directory_iterator(dir_path);

        /* for - iterate over all director entries */
        for (auto &&dir_entry : dir_iter)
        {
            m_children.push_back(dir_entry.path().relative_path().native().c_str());
        }
        /* end for - iterate over all director entries */

        SET_ERROR_CODE(ec, ERROR_CODE::NO_ERROR);
        return m_children;
    }

    std::vector<std::string> get_dir_content(const boost::filesystem::path& path, ERROR_CODE* ec)
    {
        std::vector<std::string> m_children;
        std::string path_str = path.native().c_str();

        /* if - is passed in path a directory */
        if(boost::filesystem::is_directory(path) == false)
        {
            SET_ERROR_CODE(ec, ERROR_CODE::INVALID_ARGUMENT);
            return {};
        }
        /* end if - is passed in path a directory */

        /* get a directory iterator over the directory */
        auto dir_iter = boost::filesystem::directory_iterator(path);

        /* for - iterate over all director entries */
        for (auto &&entry : dir_iter)
        {
            m_children.push_back(entry.path().relative_path().native().c_str());
        }
        /* end for - iterate over all director entries */
        
        SET_ERROR_CODE(ec, ERROR_CODE::NO_ERROR);
        return m_children;
    }

    std::string get_file_content(const std::string& path, ERROR_CODE* ec)
    {

    }

    std::string get_file_content(const boost::filesystem::path& path, ERROR_CODE* ec)
    {

    }
}