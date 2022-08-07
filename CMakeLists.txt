cmake_minimum_required(VERSION 3.16.3)
project(Cplorer C)

# Set the CMake compiler flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "-O0 -Wno-psabi -Wall")
else()
    set(CMAKE_CXX_FLAGS "-O3 -Wno-psabi -Wall")
endif()

set(CMAKE_CXX_STANDARD 17)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

# Copy the test_files into the output directory.
file(COPY test_files DESTINATION .)

add_subdirectory(src)