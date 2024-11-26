#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include "functions.h"

#define TRAIL_LENGTH 10  // Length of the trail effect
#define PLOT_DELAY 10000 // Microseconds

int main()
{
    const char *file_path = "mouse_data.dat"; // Path to the binary file
    FILE *input_file = fopen(file_path, "rb");
    if (!input_file)
    {
        perror("Error opening binary file");
        return 1;
    }

    // Print start message
    printf("Starting mouse plotter. Press Ctrl+C to stop.\n");
    sleep(1); // Brief delay

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0); // Hide the cursor

    int trail_x[TRAIL_LENGTH] = {0}; // Trail X positions
    int trail_y[TRAIL_LENGTH] = {0}; // Trail Y positions
    int trail_index = 0;             // Current trail index
    int x, y;                        // Scaled and clamped coordinates from file

    while (fread(&x, sizeof(int), 1, input_file) == 1 &&
           fread(&y, sizeof(int), 1, input_file) == 1)
    {
        // Plot the current position
        mvaddch(y, x, '*');
        refresh();

        // Clear trail character
        if (trail_index >= TRAIL_LENGTH)
        {
            int clear_x = trail_x[trail_index % TRAIL_LENGTH];
            int clear_y = trail_y[trail_index % TRAIL_LENGTH];
            mvaddch(clear_y, clear_x, ' '); // Clear old character
        }

        // Update trail
        trail_x[trail_index % TRAIL_LENGTH] = x;
        trail_y[trail_index % TRAIL_LENGTH] = y;
        trail_index++;

        usleep(PLOT_DELAY); // Control speed
    }

    if (feof(input_file))
    {
        mvprintw(0, 0, "End of data. Exiting...");
    }
    else if (ferror(input_file))
    {
        mvprintw(0, 0, "Error reading file.");
    }
    refresh();
    sleep(1);

    // Clean up
    endwin();
    fclose(input_file);
    return 0;
}
