cmake_minimum_required(VERSION 3.16.3)

set(SOURCE_FILE
        main.c
        ascii_table.h
        file_handler.c
        file_handler.h
        dir_handler.c
        dir_handler.h
        input.c
        input.h)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
link_libraries(${CURSES_LIBRARIES})

add_executable(${PROJECT_NAME} ${SOURCE_FILE})