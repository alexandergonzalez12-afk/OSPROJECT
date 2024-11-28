#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "functions.h"

// Global variables for tracking file and mouse movements
FILE *output_file = NULL;  // File to save the mouse coordinates
FILE *output_transformed_file =
    NULL;            // File to save the transformed coordinates
float min_x, max_x;  // Minimum and maximum X values
float min_y, max_y;  // Minimum and maximum Y values
float x = 0, y = 0;  // Start mouse in the middle of terminal

int main() {
  const char *device = "/dev/input/mice";  // Path to the mouse device
  int fd = open(device, O_RDONLY);
  if (fd == -1) {
    perror("Error opening mouse device");
    return 1;
  }

  // Ensure the binary file exists and open it for raw data
  output_file = fopen("mouse_raw_data.dat", "wb");
  // Open a new file to store the transformed coordinates
  output_transformed_file = fopen("mouse_data.dat", "wb");

  // Set up SIGINT handler
  signal(SIGINT, SignalHandler);

  // Create the symbolic link to mouse_plotter's build directory
  CreateSymbolicLink();

  signed char data[3];
  printf("Capturing mouse movements. Press Ctrl+C to stop.\n");

  int gate = 1;

  while (1) {
    // Read mouse data
    if (read(fd, data, sizeof(data)) > 0) {
      int dx = data[1];  // Change in X
      int dy = data[2];  // Change in Y

      // Update absolute positions
      x += dx;
      y -= dy;  // Minus because terminal Y increases downwards
      if (gate == 1) {
        gate = 0;
        min_x = x;
        max_x = x;
        min_y = y;
        max_y = y;
      } else {
        // Track min/max values for scaling
        if (x < min_x) min_x = x;
        if (x > max_x) max_x = x;
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;
      }

      // Write the raw coordinates to the file
      fwrite(&x, sizeof(float), 1, output_file);
      fwrite(&y, sizeof(float), 1, output_file);

      // Debug print
      printf(
          "Raw: X=%.2f, Y=%.2f | MAX: X=%.2f, Y=%.2f | MIN: X=%.2f, Y=%.2f\n",
          x, y, max_x, max_y, min_x, min_y);

      // Ensure data is written to disk
      fflush(output_file);
    }
  }
}
