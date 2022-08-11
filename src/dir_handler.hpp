//
// Created by wolfgang on 07/08/2022.
//

#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

DIR* open_dir(const char* dir_path);
void close_dir(DIR* dp);

void get_dir_content_str(const char* dir_path, char*** dir_content, uint32_t* n);
void get_dir_content_dp(DIR* dp, char*** dir_content, uint32_t* n);

int64_t get_dir_children_count_str(const char* dir_path);
int64_t get_dir_children_count_dp(DIR* dp);