#include "functions.h"

/**
 * @brief Simple delay function to pause execution for a specified time.
 * @param milliseconds The number of milliseconds to delay.
 */
void delay(int milliseconds) {
    usleep(milliseconds * 1000);  // Convert ms to microseconds
}
