cmake_minimum_required(VERSION 3.16.3)

set(SOURCE_FILE
        main.c
        ascii_table.h
        file_handler.c
        file_handler.h
        typedef.h
        dir_handler.c
        dir_handler.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILE})