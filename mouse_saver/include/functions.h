#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>

#define TERM_WIDTH 100
#define TERM_HEIGHT 25
// Global variables for tracking file and mouse movements

extern FILE *output_transformed_file;
extern FILE *output_file;
extern float min_x, max_x; // Minimum and maximum X values
extern float min_y, max_y; // Minimum and maximum Y values
extern float x, y;         // Start mouse in the middle of terminal

void SignalHandler(int sig);

/**
 * @brief Ensures the binary file exists. If not, it creates it and opens it for appending.
 * @param filePath The path to the binary file.
 * @return FILE* Pointer to the opened file.
 */
FILE *EnsureFileExists(const char *filePath);

/**
 * @brief Creates a symbolic link for the mouse data file in the mouse_plotter build directory.
 */
void CreateSymbolicLink();

#endif // FUNCTIONS_H
