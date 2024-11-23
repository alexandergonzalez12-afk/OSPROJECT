#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/**
 * @brief Converts pixel values to terminal coordinates.
 * @param delta The delta position of the mouse in pixels.
 * @param scale The scale factor for conversion.
 * @return The scaled coordinate in terminal units.
 */
float convert_to_terminal(float delta, float scale);

#endif  // FUNCTIONS_H
