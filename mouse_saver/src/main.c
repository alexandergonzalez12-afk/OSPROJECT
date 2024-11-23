#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "functions.h"

FILE *output_file;  // File to save the mouse coordinates

/**
 * @brief Handles the SIGINT signal to safely close the output file.
 * This ensures that the file is closed before the program exits.
 */
void handle_sigint(int sig) {
    if (output_file) {
        fclose(output_file);
        printf("\nFile closed safely. Exiting...\n");
    }
    exit(0);
}

int main() {
    const char *device = "/dev/input/mice";  // Path to the mouse device
    int fd = open(device, O_RDONLY);
    if (fd == -1) {
        perror("Error opening mouse device");
        return 1;
    }

    signed char data[3];
    float x = 50.0, y = 12.5;  // Start mouse in the middle of terminal
    float x_scale = 1.0, y_scale = 1.0;  // Scale factors (to be updated later)

    // Open the file for writing
    output_file = fopen("mouse_data.dat", "wb");
    if (!output_file) {
        perror("Error opening output file");
        close(fd);
        return 1;
    }

    // Set up SIGINT handler
    signal(SIGINT, handle_sigint);

    printf("Capturing mouse movements. Press Ctrl+C to stop.\n");

    while (1) {
        // Read mouse data
        if (read(fd, data, sizeof(data)) > 0) {
            int dx = data[1];  // Change in X
            int dy = data[2];  // Change in Y

            // Update absolute positions
            x += dx * x_scale;
            y -= dy * y_scale;  // Minus because terminal Y increases downwards

            // Clamp values to terminal size
            if (x < 0) x = 0;
            if (x > 100) x = 100;
            if (y < 0) y = 0;
            if (y > 25) y = 25;

            // Save coordinates to file
            int x_int = (int)x;
            int y_int = (int)y;
            fwrite(&x_int, sizeof(int), 1, output_file);
            fwrite(&y_int, sizeof(int), 1, output_file);

            // Debug print
            printf("X: %.2f, Y: %.2f (saved: %d, %d)\n", x, y, x_int, y_int);
        }
    }

    close(fd);
    return 0;
}
