//
// Created by wolfgang on 07/08/2022.
//

#ifndef CPLORER_DIR_HANDLER_H
#define CPLORER_DIR_HANDLER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

void get_dir_content_str(const char* dir_path, char** dir_content, uint32_t* n);
void get_dir_content_dp(DIR* dir_path, char** dir_content, uint32_t* n);

#endif // CPLORER_DIR_HANDLER_H
