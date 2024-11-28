/**
 * @file main.c
 * @brief This file contains the main function for the mouse_saver program.
 *
 * This program tracks mouse movements from the system mouse input device
 * (/dev/input/mice) and saves both the raw and transformed (scaled for terminal
 * display) coordinates to binary files. The transformation scales the
 * coordinates to fit a terminal window (x=[0,100], y=[0,25]).
 *
 * This code was created by Alexander J. Gonzalez Suarez and Alex J. Strubbe
 * Martinez.
 */

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "functions.h"

// Global variables for tracking file and mouse movements
FILE *output_file = NULL;  // File to save the mouse coordinates (raw data)
FILE *output_transformed_file = NULL;  // File to save the transformed mouse
                                       // coordinates (scaled for terminal)
float min_x, max_x;  // Tracks minimum and maximum X values for scaling
float min_y, max_y;  // Tracks minimum and maximum Y values for scaling
float x = 0, y = 0;  // Start the mouse at the center of the terminal (0, 0)

int main() {
  const char *device = "/dev/input/mice";  // Path to the mouse input device
  int fd = open(device, O_RDONLY);  // Open the mouse device in read-only mode
  if (fd == -1) {
    perror("Error opening mouse device");  // If opening the device fails, print
                                           // error
    return 1;                              // Exit the program with error code 1
  }

  // Ensure the binary file for raw data exists and open it for writing
  output_file = fopen("mouse_raw_data.dat", "wb");
  // Open a new file to store the transformed coordinates (scaled for terminal)
  output_transformed_file = fopen("mouse_data.dat", "wb");

  // Set up the SIGINT signal handler
  signal(SIGINT, SignalHandler);

  // Create a symbolic link for the mouse data file in the mouse_plotter's build
  // directory
  CreateSymbolicLink();

  signed char data[3];  // Array to store the raw data from the mouse device (X
                        // and Y delta)
  printf("Capturing mouse movements. Press Ctrl+C to stop.\n");

  int gate = 1;  // Flag to check if it's the first mouse position

  while (1) {
    // Read raw mouse data: dx (change in X), dy (change in Y)
    if (read(fd, data, sizeof(data)) > 0) {
      int dx = data[1];  // Delta X (movement in X direction)
      int dy = data[2];  // Delta Y (movement in Y direction)

      // Update the absolute positions based on deltas
      x += dx;  // Update X position
      y -= dy;  // Update Y position (inverted because terminal Y increases
                // downward)

      // On the first reading, initialize min/max values
      if (gate == 1) {
        gate = 0;
        min_x = x;  // Set initial min_x to the current X position
        max_x = x;  // Set initial max_x to the current X position
        min_y = y;  // Set initial min_y to the current Y position
        max_y = y;  // Set initial max_y to the current Y position
      } else {
        // Update the min and max values as new coordinates come in
        if (x < min_x) min_x = x;  // Update min_x if the new x is smaller
        if (x > max_x) max_x = x;  // Update max_x if the new x is larger
        if (y < min_y) min_y = y;  // Update min_y if the new y is smaller
        if (y > max_y) max_y = y;  // Update max_y if the new y is larger
      }

      // Write the current raw coordinates to the output file
      fwrite(&x, sizeof(float), 1, output_file);
      fwrite(&y, sizeof(float), 1, output_file);

      // Debugging output for the current mouse position and the min/max values
      printf(
          "Raw: X=%.2f, Y=%.2f | MAX: X=%.2f, Y=%.2f | MIN: X=%.2f, Y=%.2f\n",
          x, y, max_x, max_y, min_x, min_y);

      // Ensure data is written to disk immediately
      fflush(output_file);
    }
  }
}
