#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>

/**
 * @brief Dynamically retrieves the terminal dimensions.
 * This function uses ncurses to determine the current terminal width and height.
 * @param[out] width Pointer to store the terminal width.
 * @param[out] height Pointer to store the terminal height.
 */
void GetTerminalDimensions(int *width, int *height);

/**
 * @brief Scales the given coordinates to fit the current terminal size.
 * This function rescales coordinates from the base dimensions used during recording
 * to the dimensions of the current terminal.
 * @param[in] x Original X coordinate.
 * @param[in] y Original Y coordinate.
 * @param[in] base_width The width of the terminal during recording.
 * @param[in] base_height The height of the terminal during recording.
 * @param[in] term_width The current terminal width.
 * @param[in] term_height The current terminal height.
 * @param[out] scaled_x Pointer to store the scaled X coordinate.
 * @param[out] scaled_y Pointer to store the scaled Y coordinate.
 */
void ScaleCoordinates(int x, int y, int base_width, int base_height,
                      int term_width, int term_height, int *scaled_x, int *scaled_y);

/**
 * @brief Clamps coordinates to fit within the terminal bounds.
 * Ensures the given coordinates are within the range [0, width-1] and [0, height-1].
 * @param[in,out] x Pointer to the X coordinate to clamp.
 * @param[in,out] y Pointer to the Y coordinate to clamp.
 * @param[in] width Terminal width.
 * @param[in] height Terminal height.
 */
void ClampCoordinates(int *x, int *y, int width, int height);

#endif // FUNCTIONS_H
