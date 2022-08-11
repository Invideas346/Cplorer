//
// Created by wolfgang on 07/08/2022.
//

#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

FILE* open_file(const char* filepath, const char* mode);
void close_file(FILE* fp);

char* get_file_preview_str(const char* filepath);
char* get_file_preview_fp(FILE* fp);

char* get_file_content_str(const char* filepath);
char* get_file_content_fp(FILE* fp);

char* get_file_content_strn(const char* filepath, int64_t n);
char* get_file_content_fpn(FILE* fp, int64_t n);

int64_t get_file_size_str(const char* filepath);
int64_t get_file_size_fp(FILE* fp);