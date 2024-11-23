#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include "functions.h"

/**
 * @brief Reads coordinates from a binary file and plots them in the terminal.
 * The function uses ncurses to draw characters at specific coordinates, scaling
 * and clamping them to fit within the terminal dimensions.
 * @return 0 if the program exits successfully.
 */
int main() {
    const char *file_path = "mouse_data.dat";  // Path to the binary file
    FILE *input_file = fopen(file_path, "rb");

    if (!input_file) {
        perror("Error opening binary file");
        return 1;
    }

    // Initialize ncurses
    initscr();            // Start ncurses mode
    cbreak();             // Disable line buffering
    noecho();             // Don't show input on the screen
    curs_set(0);          // Hide the cursor

    int term_width, term_height;          // Current terminal dimensions
    int base_width = 100, base_height = 25;  // Dimensions during recording
    int x, y;                             // Raw coordinates from the file
    int scaled_x, scaled_y;               // Scaled coordinates for plotting

    printf("Plotting mouse movements. Press Ctrl+C to stop.\n");
    sleep(1);  // Wait a moment before clearing the terminal

    // Main plotting loop
    while (fread(&x, sizeof(int), 1, input_file) == 1 &&
           fread(&y, sizeof(int), 1, input_file) == 1) {
        // Dynamically get the current terminal dimensions
        GetTerminalDimensions(&term_width, &term_height);

        // Rescale coordinates to fit the current terminal size
        ScaleCoordinates(x, y, base_width, base_height, term_width, term_height, &scaled_x, &scaled_y);

        // Clamp coordinates to ensure they're within bounds
        ClampCoordinates(&scaled_x, &scaled_y, term_width, term_height);

        // Plot the rescaled and clamped coordinates
        mvaddch(scaled_y, scaled_x, '*');  // Place '*' at (scaled_x, scaled_y)
        refresh();                         // Update the screen

        // Optional debug output for raw and scaled coordinates
        printf("Plotted: Raw X=%d, Y=%d | Scaled X=%d, Y=%d\n", x, y, scaled_x, scaled_y);

        // Wait 10ms before plotting the next point
        usleep(10000);
    }

    // Clean up resources
    endwin();          // Exit ncurses mode
    fclose(input_file); // Close the binary file

    return 0;
}
