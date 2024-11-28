#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>

#define TRAIL_LENGTH 10  // Length of the trail effect



/**
 * @brief File pointer to the input binary file containing mouse data.
 */
extern FILE *input_file;

/**
 * @brief Path to the binary file containing mouse data.
 */
extern const char *file_path;

/**
 * @brief Array to store the X positions of the trail.
 */
extern int trail_x[TRAIL_LENGTH];

/**
 * @brief Array to store the Y positions of the trail.
 */
extern int trail_y[TRAIL_LENGTH];

/**
 * @brief Current index in the trail arrays.
 */
extern int trail_index;

/**
 * @brief Coordinates read from the file.
 */
extern int x, y;

// Function declarations

/**
 * @brief Signal handler function to read mouse data and update the display.
 * 
 * @param signum The signal number that triggered the handler.
 */
void DrawTrace(int signum);

#endif  // FUNCTIONS_H