#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <dispatch/dispatch.h>

/**
 * @brief Ensures the binary file exists. If not, it creates it and opens it for appending.
 * @param filePath The path to the binary file.
 * @return FILE* Pointer to the opened file.
 */
FILE *EnsureFileExists(const char *filePath);

/**
 * @brief Handles the SIGINT signal to safely close the output file and exit.
 * @param sig The signal number (SIGINT).
 */
void HandleSigint(int sig);

/**
 * @brief Sets up a GCD-based timer to trigger at regular intervals.
 * @param intervalMs Timer interval in milliseconds.
 * @param handler The callback function to invoke on timer expiry.
 * @return dispatch_source_t The created GCD timer.
 */
dispatch_source_t SetupGCDTimer(int intervalMs, void (^handler)(void));

#endif // FUNCTIONS_H
