//
// Created by wolfgang on 07/08/2022.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "file_handler.h"
#include "typedef.h"

#define PREVIEW_SIZE UINT16_MAX

static inline uint8_t validate_filepath(const char* filepath)
{
    /* if - was a valid filepath entered */
    if (filepath == NULL)
    {
        fprintf(stderr, "Enter a valid path to a file");
        return false;
    }
    if (strlen(filepath) == 0)
    {
        fprintf(stderr, "Enter a valid path to a file");
        return false;
    }
    /* end if - was a valid filepath entered */

    return true;
}

char* get_file_preview_str(const char* filepath)
{
    FILE* fp = open_file(filepath, "r");
    char* file_content = get_file_preview_fp(fp);
    close_file(fp);
    return file_content;
}

char* get_file_preview_fp(FILE* fp)
{
    char* file_content = get_file_content_fpn(fp, PREVIEW_SIZE);
    return file_content;
}

int64_t get_file_size_str(const char* filepath)
{
    FILE* fp = open_file(filepath, "r");
    int64_t file_size = get_file_size_fp(fp);
    close_file(fp);
    return file_size;
}

int64_t get_file_size_fp(FILE* fp)
{
    /* if - check if the file pointer is null */
    if (fp == NULL)
    {
        return -1;
    }
    /* end if - check if the file pointer is null */
    fseek(fp, 0L, SEEK_END);
    int64_t file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    return file_size;
}

char* get_file_content_strn(const char* filepath, int64_t n)
{
    FILE* fp = open_file(filepath, "r");
    char* file_content = get_file_content_fpn(fp, n);
    close_file(fp);
    return file_content;
}

char* get_file_content_fpn(FILE* fp, int64_t n)
{
    /* if - check if the file pointer is null */
    if (fp == NULL)
    {
        fprintf(stderr, "File pointer is NULL!");
        return NULL;
    }
    /* end if - check if the file pointer is null */

    /* get current pos to reset to status quo */
    int64_t pos = ftell(fp);

    int64_t file_size = get_file_size_fp(fp);

    /* if - check whether the file size is bigger than the preview size */
    if (file_size > n)
    {
        file_size = n;
    }
    /* end if - check whether the file size is bigger than the preview size */

    char* file_content = (char*) malloc(file_size);
    fseek(fp, 0, SEEK_SET);
    fread(file_content, sizeof(char), file_size, fp);

    /* reset to status quo */
    fseek(fp, pos, SEEK_SET);
    return file_content;
}

FILE* open_file(const char* filepath, const char* mode)
{
    /* if - validate the filepath */
    if (validate_filepath(filepath) == false)
    {
        return NULL;
    }
    /* end if - validate the filepath */

    FILE* fp = fopen(filepath, mode);

    /* if - check whether the file could be opened */
    if (fp == NULL)
    {
        fprintf(stderr, "Could not open file %s", filepath);
        return NULL;
    }
    /* end if - check whether the file could be opened */

    return fp;
}

void close_file(FILE* fp) { fclose(fp); }