#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>

/**
 * @brief Handles the SIGINT signal to safely close the output file and exit.
 * @param sig The signal number (SIGINT).
 */
void HandleSigint(int sig);

/**
 * @brief Ensures the binary file exists. If not, it creates it and opens it for appending.
 * @param filePath The path to the binary file.
 * @return FILE* Pointer to the opened file.
 */
FILE *EnsureFileExists(const char *filePath);

/**
 * @brief Creates a symbolic link for the mouse data file in the mouse_plotter build directory.
 */
void CreateSymlink();

#endif // FUNCTIONS_H
