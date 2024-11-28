/**
 * @file functions.h
 * @brief This header file contains the declarations for functions and global
 * variables used by the mouse_plotter program, which reads mouse data and
 * displays a trail effect.
 *
 * The program uses mouse data from a binary file and plots the coordinates as a
 * moving trail.
 *
 * This code was created by Alexander J. Gonzalez Suarez and Alex J. Strubbe
 * Martinez.
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>

#define TRAIL_LENGTH \
  10  // Length of the trail effect (number of previous mouse positions to
      // track)

extern FILE *input_file;  // File pointer to read mouse data from a binary file
extern const char *file_path;  // Path to the binary file containing mouse data
extern int
    trail_x[TRAIL_LENGTH];  // Array to store X coordinates for the trail effect
extern int
    trail_y[TRAIL_LENGTH];  // Array to store Y coordinates for the trail effect
extern int
    trail_index;  // Index to track the current position in the trail arrays
extern int x,
    y;  // X and Y coordinates of the mouse position read from the file

// Function declarations

/**
 * @brief Signal handler function to read mouse data and update the display.
 *
 * This function is triggered by a signal (such as a timer) to read mouse data
 * from the binary file and update the display by drawing the trail of mouse
 * movements.
 *
 * @param signum The signal number that triggered the handler.
 */
void DrawTrace(int signum);

#endif  // FUNCTIONS_H
