#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include "functions.h"

/**
 * @brief Reads coordinates from a binary file and plots them in the terminal.
 * The function uses ncurses to draw characters at specific coordinates.
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

    int x, y;

    printf("Plotting mouse movements. Press Ctrl+C to stop.\n");
    sleep(1);  // Wait a moment before clearing

    // Main plotting loop
    while (fread(&x, sizeof(int), 1, input_file) == 1 &&
           fread(&y, sizeof(int), 1, input_file) == 1) {
        // Plot the coordinate
        mvaddch(y, x, '*');  // Place '*' at (x, y)
        refresh();           // Update the screen

        // Debug print (optional, not shown in ncurses mode)
        printf("Plotted: X=%d, Y=%d\n", x, y);

        usleep(10000);  // Wait 10ms before the next point
    }

    // Clean up
    endwin();  // Exit ncurses mode
    fclose(input_file);

    return 0;
}
