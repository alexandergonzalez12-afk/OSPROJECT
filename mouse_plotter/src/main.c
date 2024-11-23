#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include "functions.h"

int main() {
    const char *filePath = "mouse_data.dat";  // Path to the binary file
    FILE *inputFile = fopen(filePath, "rb");

    if (!inputFile) {
        perror("Error opening binary file");
        return 1;
    }

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    int termWidth, termHeight;
    int baseWidth = 100, baseHeight = 25;  // Assumed dimensions during recording
    int x, y;

    // Set up SIGINT handler
    signal(SIGINT, HandleSigint);

    // GCD Timer for periodic plotting
    SetupGCDTimer(10, ^{
        if (fread(&x, sizeof(int), 1, inputFile) == 1 && fread(&y, sizeof(int), 1, inputFile) == 1) {
            // Get the current terminal dimensions
            getmaxyx(stdscr, termHeight, termWidth);

            // Rescale coordinates to fit the current terminal dimensions
            int scaledX = x * termWidth / baseWidth;
            int scaledY = y * termHeight / baseHeight;

            // Clamp coordinates to ensure they're within bounds
            if (scaledX < 0) scaledX = 0;
            if (scaledX >= termWidth) scaledX = termWidth - 1;
            if (scaledY < 0) scaledY = 0;
            if (scaledY >= termHeight) scaledY = termHeight - 1;

            // Plot the coordinate
            mvaddch(scaledY, scaledX, '*');
            refresh();

            // Debug print (optional)
            printf("Plotted: Raw X=%d, Y=%d | Scaled X=%d, Y=%d\n", x, y, scaledX, scaledY);
        } else {
            // End plotting if no more data
            endwin();
            fclose(inputFile);
            exit(0);
        }
    });

    // Block main thread to keep ncurses running
    pause();

    // Clean up
    endwin();
    fclose(inputFile);
    return 0;
}
