#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "functions.h"

// Global variables for tracking file and mouse movements
FILE *output_file = NULL;       // File to save the mouse coordinates
float min_x = 0, max_x = 0;     // Minimum and maximum X values
float min_y = 0, max_y = 0;     // Minimum and maximum Y values

int main() {
    const char *device = "/dev/input/mice";  // Path to the mouse device
    int fd = open(device, O_RDONLY);
    if (fd == -1) {
        perror("Error opening mouse device");
        return 1;
    }

    float x = 50.0, y = 12.5;  // Start mouse in the middle of terminal
    int term_width = 100, term_height = 25;  // Terminal dimensions

    // Ensure the binary file exists and open it
    output_file = EnsureFileExists("mouse_data.dat");

    // Set up SIGINT handler
    signal(SIGINT, HandleSigint);

    // Create the symbolic link to mouse_plotter's build directory
    CreateSymlink();

    signed char data[3];
    printf("Capturing mouse movements. Press Ctrl+C to stop.\n");

    while (1) {
        // Read mouse data
        if (read(fd, data, sizeof(data)) > 0) {
            int dx = data[1];  // Change in X
            int dy = data[2];  // Change in Y

            // Update absolute positions
            x += dx;
            y -= dy;  // Minus because terminal Y increases downwards

            // Track min/max values for scaling
            if (x < min_x) min_x = x;
            if (x > max_x) max_x = x;
            if (y < min_y) min_y = y;
            if (y > max_y) max_y = y;

            // Calculate scaling factors
            float x_scale = (max_x - min_x) > 0 ? (float)term_width / (max_x - min_x) : 1.0;
            float y_scale = (max_y - min_y) > 0 ? (float)term_height / (max_y - min_y) : 1.0;

            // Scale to terminal dimensions
            float scaled_x = (x - min_x) * x_scale;
            float scaled_y = (y - min_y) * y_scale;

            // Clamp values to terminal size
            if (scaled_x < 0) scaled_x = 0;
            if (scaled_x > term_width) scaled_x = term_width;
            if (scaled_y < 0) scaled_y = 0;
            if (scaled_y > term_height) scaled_y = term_height;

            // Save coordinates to file
            int x_int = (int)scaled_x;
            int y_int = (int)scaled_y;
            fwrite(&x_int, sizeof(int), 1, output_file);
            fwrite(&y_int, sizeof(int), 1, output_file);

            // Debug print
            printf("Raw: X=%.2f, Y=%.2f | Scaled: X=%.2f, Y=%.2f | Saved: (%d, %d)\n",
                   x, y, scaled_x, scaled_y, x_int, y_int);
        }
    }

    close(fd);
    return 0;
}
