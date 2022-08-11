//
// Created by wolfgang on 07/08/2022.
//

#include "dir_handler.hpp"
#include <stdbool.h>
#include <string.h>

static inline uint8_t validate_dirpath(const char* dir_path)
{
    /* if - was a valid filepath entered */
    if (dir_path == NULL)
    {
        fprintf(stderr, "Enter a valid path to a directory");
        return false;
    }
    if (strlen(dir_path) == 0)
    {
        fprintf(stderr, "Enter a valid path to a directory");
        return false;
    }
    /* end if - was a valid filepath entered */

    return true;
}

DIR* open_dir(const char* dir_path)
{
    /* if - validate the dirpath */
    if (validate_dirpath(dir_path) == false)
    {
        return NULL;
    }
    /* end if - validate the dirpath */

    DIR* dp = opendir(dir_path);

    /* if - check whether the directory could be opened */
    if (dp == NULL)
    {
        fprintf(stderr, "Could not open directory %s", dir_path);
        return NULL;
    }
    /* end if - check whether the directory could be opened */

    return dp;
}

void close_dir(DIR* dp) { closedir(dp); }

void get_dir_content_str(const char* dir_path, char*** dir_content, uint32_t* n)
{
    DIR* dp = open_dir(dir_path);
    get_dir_content_dp(dp, dir_content, n);
    close_dir(dp);
}

void get_dir_content_dp(DIR* dp, char*** dir_content, uint32_t* n)
{
    struct dirent* entry;
    int64_t children_count = get_dir_children_count_dp(dp);
    *n = 0;
    (*dir_content) = (char**) malloc(sizeof(char*) * children_count);
    while ((entry = readdir(dp)))
    {
        (*n)++;
        (*dir_content)[*n] = (char*) malloc(strlen(entry->d_name) + 1);
        memcpy((*dir_content)[*n], entry->d_name, strlen(entry->d_name) + 1);
    }
}

int64_t get_dir_children_count_str(const char* dir_path) { return 0; }

int64_t get_dir_children_count_dp(DIR* dp)
{
    /* if - check if directory pointer is valid */
    if (dp == NULL)
    {
        fprintf(stderr, "Dir. pointer is NULL");
        return -1;
    }
    /* end if - check if directory pointer is valid */

    struct dirent* entry;
    int64_t children_count = 0;

    /* while - get children count */
    while ((entry = readdir(dp)))
    {
        children_count++;
    }
    /* end while - get children count */

    rewinddir(dp);
    return children_count;
}
