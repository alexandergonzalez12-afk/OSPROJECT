cmake_minimum_required(VERSION 3.10)

# Project name
project(MouseSaver)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directory
include_directories(include)

# Source files
set(SOURCES
    src/main.c
    src/functions.c
)

# Add executable
add_executable(mouse_saver ${SOURCES})
