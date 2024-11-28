/**
 * @file functions.h
 * @brief This file contains declarations for helper functions used by the
 * mouse_saver program.
 *
 * The functions include handling signals, creating
 * symbolic links, and processing mouse data transformations.
 *
 * This code was created by Alexander J. Gonzalez Suarez and Alex J. Strubbe
 * Martinez.
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>

// Terminal dimensions for coordinate scaling
#define TERM_WIDTH 100  ///< Width of the terminal (columns)
#define TERM_HEIGHT 25  ///< Height of the terminal (rows)

// Global variables for tracking file and mouse movements

extern FILE *output_transformed_file;  ///< Pointer to the file for storing
                                       ///< transformed mouse coordinates
extern FILE
    *output_file;  ///< Pointer to the file for storing raw mouse coordinates

extern float min_x;  ///< Tracks the minimum X-coordinate encountered during
                     ///< mouse movement
extern float max_x;  ///< Tracks the maximum X-coordinate encountered during
                     ///< mouse movement
extern float min_y;  ///< Tracks the minimum Y-coordinate encountered during
                     ///< mouse movement
extern float max_y;  ///< Tracks the maximum Y-coordinate encountered during
                     ///< mouse movement

extern float x;  ///< Current X-coordinate of the mouse in absolute terms
extern float y;  ///< Current Y-coordinate of the mouse in absolute terms

/**
 * @brief Handles SIGINT (Ctrl+C) signal to safely close files and exit the
 * program.
 *
 * @param sig The signal number.
 */
void SignalHandler(int sig);

/**
 * @brief Creates a symbolic link for the mouse data file in the mouse_plotter
 * build directory.
 */
void CreateSymbolicLink();

#endif  // FUNCTIONS_H
