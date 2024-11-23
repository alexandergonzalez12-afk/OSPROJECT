cmake_minimum_required(VERSION 3.10)

# Project name
project(MousePlotter)

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

# Link ncurses library
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
set(LIBRARIES ${CURSES_LIBRARIES})

# Add executable
add_executable(mouse_plotter ${SOURCES})
target_link_libraries(mouse_plotter ${LIBRARIES})

# Custom command to create symbolic link
add_custom_command(
    TARGET mouse_plotter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_SOURCE_DIR}/../mouse_saver/mouse_data.dat
    ${CMAKE_BINARY_DIR}/mouse_data.dat
    COMMENT "Creating symbolic link to mouse_data.dat"
)
