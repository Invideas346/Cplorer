cmake_minimum_required(VERSION 3.16.3)

set(SOURCE_FILE
        input.hpp 
        input.cpp
        fs_handler.hpp 
        fs_handler.cpp
        cli.hpp 
        cli.cpp
        config.hpp 
        config.cpp
        application.hpp 
        application.cpp
        main.cpp)
        
# Find boost
find_package(Boost REQUIRED COMPONENTS random system thread filesystem chrono atomic date_time regex)

# Find curses
find_package(Curses REQUIRED)

# Add the library to the build system
include_directories(${CURSES_INCLUDE_DIR})
link_libraries(${CURSES_LIBRARIES})

# Set the C++ 
add_executable(${PROJECT_NAME} ${SOURCE_FILE})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

link_libraries(plog ${CMAKE_BINARY_DIR}/plog)
include_directories(${CMAKE_SOURCE_DIR}/plog/include)

link_libraries(json ${CMAKE_BINRAY_DIR}/json)
include_directories(${CMAKE_SOURCE_DIR}/json/single_include)

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