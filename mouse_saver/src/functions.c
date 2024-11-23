#include "functions.h"

/**
 * @brief Converts pixel values to terminal coordinates.
 * @param delta The delta position of the mouse in pixels.
 * @param scale The scale factor for conversion.
 * @return The scaled coordinate in terminal units.
 *
 * @note This is a helper function for scaling mouse movements.
 */
float convert_to_terminal(float delta, float scale) {
    return delta * scale;
}
