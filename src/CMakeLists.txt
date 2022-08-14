cmake_minimum_required(VERSION 3.16.3)

set(SOURCE_FILE
        main.cpp)
        
# Find boost
find_package(Boost REQUIRED COMPONENTS random system thread filesystem chrono atomic date_time regex)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
link_libraries(${CURSES_LIBRARIES})

add_executable(${PROJECT_NAME} ${SOURCE_FILE})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_library(input input.hpp input.cpp)
target_link_libraries(${PROJECT_NAME} input)

add_library(fs_handler fs_handler.hpp fs_handler.cpp)
target_link_libraries(${PROJECT_NAME} fs_handler)

add_library(cli cli.hpp cli.cpp)
target_link_libraries(${PROJECT_NAME} cli)

link_libraries(plog ${CMAKE_BINARY_DIR}/plog)
include_directories(${CMAKE_SOURCE_DIR}/plog/include)

target_link_libraries(${PROJECT_NAME} 
  Boost::boost
  Boost::random
  Boost::system
  Boost::thread
  Boost::filesystem
  Boost::chrono
  Boost::atomic
  Boost::date_time
  Boost::regex
)